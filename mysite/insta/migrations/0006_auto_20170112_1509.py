# -*- coding: utf-8 -*-
# Generated by Django 1.10.2 on 2017-01-12 14:09
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('insta', '0005_auto_20170112_1508'),
    ]

    operations = [
        migrations.AlterField(
            model_name='image',
            name='data',
            field=models.ImageField(upload_to='.\\insta\\static\\images'),
        ),
    ]
