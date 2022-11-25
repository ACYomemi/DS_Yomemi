# -*- coding: gbk -*-
from django.db import models
from django.db.models.signals import post_save, pre_delete
from django.dispatch import receiver
from Yomemi.uselib import *


# Create your models here.


class StudentInfo(models.Model):
    studentID = models.CharField(primary_key=True, max_length=13)  # 学工号
    studentName = models.CharField(max_length=10)  # 姓名
    pwd = models.DecimalField(max_digits=6, decimal_places=0)  # 密码的哈希值
    dept = models.CharField(max_length=2)  # 所属院系，例如CS
    totalScore = models.DecimalField(max_digits=3, decimal_places=0, default=0)
    compulsoryScore = models.DecimalField(max_digits=3, decimal_places=0, default=0)
    totalGPA = models.DecimalField(max_digits=3, decimal_places=2, default=0)
    compulsoryGPA = models.DecimalField(max_digits=3, decimal_places=2, default=0)


class TeacherInfo(models.Model):
    teacherID = models.CharField(primary_key=True, max_length=13)  # 学工号
    teacherName = models.CharField(max_length=10)  # 姓名
    pwd = models.DecimalField(max_digits=6, decimal_places=0)  # 密码的哈希值


class CourseInfo(models.Model):
    courseID = models.CharField(primary_key=True, max_length=12)  # 课程号，12位数
    courseName = models.CharField(max_length=20)  # 课程名
    score = models.DecimalField(max_digits=2, decimal_places=0)  # 学分，在0-10之间
    teacherID = models.ForeignKey('TeacherInfo', related_name='teacherID_CourseInfo',
                                  on_delete=models.CASCADE)  # 开课教师编号，映射到用户表中的姓名字段，级联更新删除
    descript = models.TextField(null=True, blank=True)  # 课程描述，可以留空

    class Meta:
        constraints = \
            [
                models.CheckConstraint(check=models.Q(score__gte=0) & models.Q(score__lte=10),
                                       name='CourseInfoScore')
            ]


class Compulsory(models.Model):
    dept = models.CharField(max_length=2)
    course = models.CharField(max_length=9)


class CourseSelect(models.Model):
    studentID = models.ForeignKey('StudentInfo', related_name='studentID_CourseSelect',
                                  on_delete=models.CASCADE)  # 选课的学号，映射到用户表中的学工号字段，级联更新删除
    courseID = models.ForeignKey('CourseInfo', related_name='courseID_CourseSelect',
                                 on_delete=models.CASCADE)  # 选课的课号，映射到课程表中的课号字段，级联更新删除
    grade = models.DecimalField(max_digits=3, decimal_places=0, null=True, blank=True)  # 分数在0至100之间，允许一位小数，可以留空
    class Meta:
        unique_together = ('studentID', 'courseID')
        constraints = \
            [
                models.CheckConstraint(check=models.Q(grade=None) | models.Q(grade__gte=0) & models.Q(grade__lte=100),
                                       name='CourseSelectGrade')
            ]


@receiver(post_save, sender=CourseSelect)
def post_save_CourseSelect(sender, instance, created, **kwargs):
    if instance.grade is not None and instance.grade >= 60:
        target = instance.studentID
        score = instance.courseID.score
        target.totalGPA = (target.totalGPA * target.totalScore + score * GPA(instance.grade)) / (
                target.totalScore + score)
        target.totalScore = target.totalScore + score
        if Compulsory.objects.filter(dept=target.dept, course=instance.courseID.courseID[0:9]).exists():
            target.compulsoryGPA = (target.compulsoryGPA * target.compulsoryScore + score * GPA(instance.grade)) / (
                    target.compulsoryScore + score)
            target.compulsoryScore = target.compulsoryScore + score
        target.save()
    return


@receiver(pre_delete, sender=CourseSelect)
def pre_delete_CourseSelect(sender, instance, **kwargs):
    if instance.grade is not None and instance.grade >= 60:
        target = instance.studentID
        score = instance.courseID.score
        if target.totalScore - score > 0:
            target.totalGPA = (target.totalGPA * target.totalScore - score * GPA(instance.grade)) / (
                    target.totalScore - score)
        else:
            target.totalGPA = 0
        target.totalScore = target.totalScore - score
        if Compulsory.objects.filter(dept=target.dept, course=instance.courseID.courseID[0:9]).exists():
            if target.compulsoryScore - score > 0:
                target.compulsoryGPA = (target.compulsoryGPA * target.compulsoryScore - score * GPA(instance.grade)) / (
                        target.compulsoryScore - score)
            else:
                target.compulsoryGPA = 0
            target.compulsoryScore = target.compulsoryScore - score
        target.save()
    return


class Notice(models.Model):
    ID = models.AutoField(primary_key=True)
    teacherID = models.ForeignKey('TeacherInfo', related_name='studentID_Notice', on_delete=models.CASCADE)
    title = models.CharField(max_length=15, default='Notice')
    text = models.TextField(null=True, blank=True)
    timestamp = models.DateTimeField(auto_now=True)
