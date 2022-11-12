# -*- coding: gbk -*-
from django.core.exceptions import ObjectDoesNotExist
from django.shortcuts import render, redirect
from Yomemi.models import *


# 请记得把这里的import语句前缀改成自己的APP名。
# Create your views here.
def login(request):
    # 登录页面
    if request.session.get('is_login'):
        return redirect('../index/')  # 已登录的玩家重定向到index
    if request.method == 'GET':
        message = None
        if request.session.get('message'):
            message = request.session.get('message')
            request.session['message'] = None
            # 这个if语句实质上是在判定其他网页有没有请求展示弹窗，如果不需要弹窗，后续可以删除
        return render(request, 'login.html', {'message': message})
    # login.html是登录页面，后续文件名都会改动，下同。
    if request.method == 'POST':
        # 下面的get语句中的user等名称也会替换，下同。
        mail = request.POST.get('user')
        pwd = request.POST.get('pwd')
        try:
            user = UserInfo.objects.get(userID=mail)
            # 使用get方法查找特定的用户。
            # 在get方法下，如果数据库中没有匹配的行(即用户不存在)，会抛出一个名为ObjectDoesNotExist的异常，必须加catch捕获异常。
            # 我打算把加密的方法写在前端。
            if pwd == user.pwd:
                request.session['is_login'] = True
                request.session['user'] = mail
                request.session.set_expiry(0)
                # 设置session对话，分别为设置登录状态，定义唯一标识符和设置过期时间，此处过期时间为关闭浏览器时。
                return redirect('../index/')
            else:
                return render(request, 'login.html', {'message': '用户名或密码错误'})
        except ObjectDoesNotExist:
            return render(request, 'login.html', {'message': '用户名或密码错误'})
        except Exception as e:
            print(repr(e))
            return render(request, 'login.html', {'message': '发生内部错误，请联系管理员'})


def modify(request):
    if not request.session.get('is_login'):
        request.session['message'] = '登录信息已失效，请重新登入'
        return redirect('../login/')
    # 更改个人信息
    # 对于密码这一特殊字段，类似于登录界面，我也计划把密码验证、取哈希值这一操作放在前端的js代码中。
    if request.method == 'POST':
        new_info = {}
        if request.POST.get('userID') != '':
            new_info['userID'] = request.POST.get('userID')
        if request.POST.get('nickName') != '':
            new_info['nickName'] = request.POST.get('nickName')
        if request.POST.get('pwd') != '':
            new_info['pwd'] = int(request.POST.get('pwd'))
        UserInfo.objects.filter(userID=request.session['user']).update(**new_info)
        # new_info是一个字典，django支持以字典形式更新信息

def logout(request):
    request.session.flush()
    # 清空session中的所有字段，即删除登录信息，用户变为未登录
    request.session['message'] = '登出成功，请重新登入'
    return redirect('../login')


def index(request):
    # 个人主页
    if not request.session.get('is_login'):
        # 获取session的is_login字段，如果字段不存在则代表登录信息过期
        # 注意！这里绝对不能用request.session['is_login']来代替，因为session默认并没有这一字段，
        # 当未登录时，直接以字典形式获取这个字段会报错！
        request.session['message'] = '登录信息已失效，请重新登入'
        return redirect('../login/')
    user = UserInfo.objects.get(userID=request.session['user'])
    return render(request, 'index.html', {'nickName': user.nickName, 'score': user.userScore})
    # 这里按照后续的业务逻辑会再做调整，


def rank(request):
    # 排名信息
    lst = UserInfo.objects.all().order_by('-userScore').values('nickName', 'userScore')
    # 按分数排序的玩家清单
    if request.method == 'GET':
        # 每页20条信息，所以做了一个切片
        # page是目前展示的页码，排名数据也需要用到它
        return render(request, 'rank.html', {'user_list': lst[:20], 'page': 1})
    if request.method == 'POST':
        if 'first' in request.POST:
            return render(request, 'rank.html', {'user_list': lst[:20], 'page': 1})
        elif 'last' in request.POST:
            return render(request, 'rank.html', {'user_list': lst[-20:], 'page': (lst.count() / 20) + 1})
        else:
            # 对于跳转到上一页下一页或者指定页的情况，我计划用js把数据处理一下之后放在下面那个名为page的文本框里，作为要跳转的页码数
            page = request.POST.get('page')
            return render(request, 'rank.html', {'user_list': lst[page * 20:page * 20 + 20], 'page': page})
