from django.conf.urls import url
from . import views

app_name = 'insta'

urlpatterns = [
    url(r'^(?P<album_id>[0-9]+)/$', views.displayAlbumView),
    url(r'^contact/$', views.createProfileView),
    url(r'^thanks/$', views.ThanksView.as_view()),
    url(r'^profilepage/$', views.profilePageView),
    url(r'^homepage/$', views.homepageView),
    url(r'^upload_file/$', views.requestView),
    url(r'^add_picture/$', views.addPictureView),
    url(r'^login/$', views.loginView),
    url(r'^logout/$', views.logoutView),
    url(r'^create_album/$', views.createAlbumView),
    url(r'^actualise_database/$', views.actualiseDatabaseView)
]

