from django.db import models
from django.contrib.auth.models import User


# Create your models here.

class Profile(models.Model):
	"""docstring for Profile
	Classe d'utilisateur qui permetera d'identifier l'utilisateur avec son nom,prenom,adresse_mail ...
	"""
	last_request = models.ImageField(upload_to='request', blank=True, null=True)
	user = models.OneToOneField(User)  # La liaison OneToOne vers le modèle User
	username = models.CharField(max_length=200, blank=True)
	def __str__(self):
		return self.username

class Image(models.Model):
	"""docstring for Image
	Clase image qui permet de la stocker avec son auteur, sa date de publication
	"""
	data = models.ImageField(upload_to='media/', blank=False)
	pub_date = models.DateTimeField('date published', blank=True, null=True)
	autor = models.ForeignKey('Profile', blank=True, null=True)
	album = models.ForeignKey('Album', blank=False)
	def __str__(self):
		return self.autor.username +" - "+ self.album.name

class Request(models.Model):
	"""docstring for Image
	Clase image qui permet de la stocker avec son auteur, sa date de publication
	"""
	data = models.ImageField(upload_to='requests/', blank=False)


class Album(models.Model):
	"""docstring for Album
	Classe qui identifie un album"""
	name = models.CharField(max_length=200)
	def __str__(self):
		return self.name

class ProfileAlbum(models.Model):
	"""docstring for ProfileAlbum
	Classe pour relier un profile a un album
"""
	profile_id = models.ForeignKey('Profile')
	album_id = models.ForeignKey('Album')
		