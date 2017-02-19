import os
import PIL
from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.http import Http404
from django.contrib.auth.models import User,AnonymousUser
from django.contrib.auth import authenticate, logout, login
from django.views.generic import TemplateView
from .models import Image, Album, Profile, ProfileAlbum, Request
from .forms import ContactForm, UploadFileForm, AddPictureForm, LoginForm, CreateAlbumForm
from .executable import request_scores, load_db, request_path, ranking, load_db_path, images_path



VALID_EXTENSIONS = ["JPEG", "PNG"]
MAXIMUM_WIDTH = 1000
MAXIMUM_HEIGHT = 700

def checkValidImage(image):
    """docstring for CheckValidImage
Fonction qui verifie l'extension de l'image limité à JPEG et PNG et qui modifie sa taille si elle est trop grande    """
    data = PIL.Image.open(image.data.path)
    if data.format not in VALID_EXTENSIONS:
        return False
    else:
        if data.size[0] > MAXIMUM_WIDTH or data.size[1] > MAXIMUM_HEIGHT:
            ratio = data.size[0]/data.size[1]
            if ratio < 1:
                height = MAXIMUM_HEIGHT
                width = int(ratio*MAXIMUM_HEIGHT)
            else: 
                width = MAXIMUM_WIDTH
                height = int(MAXIMUM_WIDTH/ratio)
            data = data.resize((width, height), PIL.Image.ANTIALIAS)
            data.save(image.data.path)
            return True

def displayAlbumView(request, album_id):
    """docstring for displayAlbum
Methode pour afficher un album avec son identifiant    """
    try:
        user=request.user
        if user.is_anonymous():
            return HttpResponseRedirect('../homepage')
        try:
            profile = Profile.objects.get(user=user, username=user.username)
            albumsprofile = ProfileAlbum.objects.all().filter(profile_id=profile)
            albums = []
            for item in albumsprofile:
                albums.append(item.album_id)    
        except Profile.DoesNotExist:
            raise Http404("Vous n'êtes pas connecté a un profil utilisateur")
        album = Album.objects.get(id=album_id)
        if (album not in albums):
            raise Http404("Vous n'avez pas le droit d'acceder a cet album")
        images = Image.objects.all().filter(album=album)
        return render(request, "display_album.html", {"images": images})
    except Album.DoesNotExist:
        raise Http404("L'album n'existe pas !")

class ThanksView(TemplateView):
    """docstring for thanks: Classe pour afficher un message de remerciement  """
    template_name = "thanks.html"


def requestView(request):
    """docstring for upload_file
Methode uploader le fichier et lancer la recherche  """

    if request.method == 'POST':
        form = UploadFileForm(request.POST, request.FILES)
        if form.is_valid():
            image_received = Request(data=form.cleaned_data['file'])
            image_received.save()
            if checkValidImage(image_received):
                request_scores(request_path, image_received.data.path)
                ranks = []
                ranking(ranks, 5)
                images = Image.objects.all()
                images_to_send = []
                for (path, score) in ranks:
                    for image in images:
                        print('image.data.path' + image.data.path)
                        print('path' + path)
                        if image.data.path == path:
                            images_to_send.append(image)
                os.remove(image_received.data.path)
                image_received.delete()
                return render(request, "display_album.html", {"images": images_to_send})
            else:
                return HttpResponseRedirect("../upload_file")
    else:
        form = UploadFileForm()
    return render(request, 'upload.html', {'form': form})

def addPictureView(request):
    """docstring for add_picture
Methode pour ajouter une image dans un abum  """
    if request.method == 'POST':
        form = AddPictureForm(request.POST, request.FILES, current_user=request.user)
        if form.is_valid():
            try:
                profile = Profile.objects.get(user=request.user, username=request.user.username)
            except Profile.DoesNotExist:
                raise Http404("Vous n'etes pas connecté a un profil utilisateur")
            new_image = Image(data=form.cleaned_data["data"], pub_date=form.cleaned_data['pub_date'], autor=profile, album=Album.objects.get(id=form.cleaned_data["album"]))
            new_image.save()
            if checkValidImage(new_image):
                return HttpResponseRedirect("../profilepage")
            else:
                os.remove(new_image.data.path)
                new_image.delete()
                form = AddPictureForm(current_user=request.user)
                return render(request, 'add_picture.html', {'form': form})
    else:
        form = AddPictureForm(current_user=request.user)
    return render(request, 'add_picture.html', {'form': form})

def createProfileView(request):
    """docstring for createProfile
Methode pour créer un profil a partir d'un formulaire  """
    if request.method == 'POST':
        form = ContactForm(request.POST)
        if form.is_valid():
            first_name = form.cleaned_data['first_name']
            last_name = form.cleaned_data['last_name']
            email = form.cleaned_data['email']
            password = form.cleaned_data['password']
            username = form.cleaned_data['username']
            user = User.objects.create_user(first_name=first_name, last_name=last_name, email=email, password=password, username=username)
            user.save()
            profile = Profile.objects.create(user=user, username=username)
            profile.save()
            return render(request, 'homepage.html')
    else:
        form = ContactForm()
    return render(request, 'contactform.html', {'form': form})


def loginView(request):
    """docstring for login_view Methode pour se login   """
    wrong_user = True
    if request.method == 'POST':
        form = LoginForm(request.POST)
        if form.is_valid():
            user = authenticate(username=form.cleaned_data['username'], password=form.cleaned_data['password'])
            if user is not None:
                login(request, user)
                return HttpResponseRedirect('../homepage')
            else:
                return render(request, 'login.html', {'wrong_user':wrong_user, 'form':form})
    else:
        form = LoginForm()
        wrong_user = False
        return render(request, 'login.html', {'wrong_user':wrong_user, 'form':form})

def createAlbumView(request):
    """docstring for login_view Methode pour se login   """
    if request.method == 'POST':
        form = CreateAlbumForm(request.POST)
        if form.is_valid():
            user = request.user
            if user.is_anonymous():
                return HttpResponseRedirect('../homepage')
            profile = Profile.objects.get(user=user, username=user.username)
            temp_album = Album(name=form.cleaned_data['name'])
            temp_album.save()
            profilealbum = ProfileAlbum(album_id=temp_album, profile_id=profile)
            profilealbum.save()
            return HttpResponseRedirect("../profilepage")
    
    else:
        form = CreateAlbumForm()
        return render(request, 'create_album.html', {'form':form})


def logoutView(request):
    """docstring for login_view
Methode pour se logout   """
    logout(request)
    return HttpResponseRedirect('../homepage')


def profilePageView(request):
    """docstring for login_view
Methode pour afficher une page de profil avec les albums d'un utilisateur et ses photos   """
    user = request.user
    if user.is_anonymous():
        return HttpResponseRedirect('../homepage')
    profile = Profile.objects.get(user=user, username=user.username)
    albumsprofile = ProfileAlbum.objects.all().filter(profile_id=profile)
    albums = []
    for item in albumsprofile:
        albums.append(item.album_id)
    photos = Image.objects.all().filter(autor=profile)
    return render(request, 'profilepage.html', {'albums':albums, 'photos':photos})


def homepageView(request):
    """docstring for login_view
Methode pour afficher la page d'acceuil   """
    return render(request, 'homepage.html')

def actualiseDatabaseView(request):
    """docstring for login_view
Methode pour actualiser la base de recherche d'images   """
    load_db(images_path, load_db_path)
    return render(request, "homepage.html")



