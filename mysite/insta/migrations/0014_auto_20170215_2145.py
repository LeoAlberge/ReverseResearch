# -*- coding: utf-8 -*-
# Generated by Django 1.10.2 on 2017-02-15 20:45
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('insta', '0013_auto_20170130_1637'),
    ]

    operations = [
        migrations.AlterField(
            model_name='image',
            name='album',
            field=models.ForeignKey(blank=True, null=True, on_delete=django.db.models.deletion.CASCADE, to='insta.Album'),
        ),
    ]
