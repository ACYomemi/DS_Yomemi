from django.shortcuts import render
from django.http import HttpResponse
import threading
# Create your views here.
write_dict = threading.Semaphore()
game_dict = {}


def gaming(request):
    if request.method == 'POST':
        game_id = request.POST['game_id']
        if request.POST['game_state'] == '1':  # 下棋方
            with write_dict:
                game_dict[game_id] = (request.POST['new_move'], request.POST['party'])
            return HttpResponse(status=200)
        else:  # 等待方
            new_move, party = game_dict.get(game_id, ('-', '-'))
            if new_move == '-' or request.POST['party'] == party:
                return HttpResponse(status=403)
            else:
                with write_dict:
                    game_dict[game_id] = ('-', '-')
                return HttpResponse(str(new_move))

    else:
        return render(request, 'test_page.html')
