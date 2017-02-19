from django.contrib import admin
from .models import Album,Profile,ProfileAlbum,Image

admin.site.register(Album)
admin.site.register(Profile)
admin.site.register(Image)
admin.site.register(ProfileAlbum)
# Register your models here.
