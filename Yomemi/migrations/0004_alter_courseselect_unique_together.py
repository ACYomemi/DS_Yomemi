# Generated by Django 4.1.2 on 2022-11-25 01:36

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('Yomemi', '0003_notice_title'),
    ]

    operations = [
        migrations.AlterUniqueTogether(
            name='courseselect',
            unique_together={('studentID', 'courseID')},
        ),
    ]
