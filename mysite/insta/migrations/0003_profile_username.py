# -*- coding: utf-8 -*-
# Generated by Django 1.10.2 on 2017-01-10 10:07
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('insta', '0002_auto_20170110_1058'),
    ]

    operations = [
        migrations.AddField(
            model_name='profile',
            name='username',
            field=models.CharField(blank=True, max_length=200),
        ),
    ]