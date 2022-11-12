# -*- coding: gbk -*-
from django.db import models


# Create your models here.

class UserInfo(models.Model):
    userID = models.EmailField(primary_key=True, max_length=15)  # 用户ID，是一个字符串
    nickName = models.CharField(max_length=15, default=userID)  # 用户昵称
    pwd = models.DecimalField(max_digits=6, decimal_places=0)  # 密码哈希
    userScore = models.DecimalField(max_digits=6, decimal_places=0, default=0)  # 用户得分


class GameInfo(models.Model):
    playerList = models.ManyToManyField('UserInfo')  # 参加对局的二人
    win = models.BooleanField(null=True, blank=True, default=None)  # 是谁赢了对局
    gameScore = models.DecimalField(max_digits=3, decimal_places=0)  # 本局的分数
