from django import forms
from django.contrib.admin import widgets                                       
from insta.models import Profile, ProfileAlbum

class ContactForm(forms.Form):
    """docstring for ContactForm :
    Formulaire pour créer un profil utilisateur""" 
    username = forms.CharField(max_length=100)
    password = forms.CharField(max_length=100, widget=forms.PasswordInput)
    first_name = forms.CharField(max_length=100)
    last_name = forms.CharField(max_length=100)
    email = forms.EmailField()

class LoginForm(forms.Form):
    """docstring for LoginForm :
    Formulaire pour se loger au site""" 
    username = forms.CharField(max_length=100)
    password = forms.CharField(max_length=100, widget=forms.PasswordInput)

class UploadFileForm(forms.Form):
    """docstring for UploadFileForm :
    Formulaire pour uploader une image requete""" 
    file = forms.ImageField()

class AddPictureForm(forms.Form):
    """docstring for AddPictureForm : Formulaire pour ajouter une image
    qui gere dynamiquement l'utilisateur""" 
    def __init__(self, *args, **kwargs):
        current_user = kwargs.pop('current_user', None)
        super(AddPictureForm, self).__init__(*args, **kwargs)
        profile = Profile.objects.get(user=current_user)
        albumsprofile = ProfileAlbum.objects.all().filter(profile_id=profile)
        albums = []
        for item in albumsprofile:
            albums.append(item.album_id)
        self.fields['album'] = forms.ChoiceField(choices=[(album.id, album.name) for album in albums])
    data = forms.ImageField()
    pub_date = forms.DateTimeField(widget=widgets.AdminDateWidget())
        
class CreateAlbumForm(forms.Form):
    """docstring for CreateAlbumForm : Formulaire pour créer un album"""
    name = forms.CharField(max_length=50)
        