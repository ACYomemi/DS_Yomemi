$("#search").click(
    function ()
    {
        if ($("#keyword").val()==="")
        {
            window.location.pathname = "search";
        }
        else
        {
            window.location.pathname = "search&keyword="+$("#keyword").val();
        }
    }
)
function callback(data,status)
{
    let dat = JSON.parse(data);
    let msg="";
    if (dat["len"]===0)
        msg = "选课操作成功";
    else
    {
        msg = dat["len"]+"个课程选择失败，失败的课程号如下。\n";
        for (let i=0;i<dat["list"].length;i++)
        {
            msg+=(dat['list'][i]+"\n");
        }
        msg+="请确保您之前从未选过这些课程。";
    }
    alert(msg);
}
$("#select").click(
    function ()
    {
        let list = [];
        let $_check = $(".check");
        let $_course = $(".course");
        for (let i=0;i<$_check.length;i++)
        {
            if ($_check.get(i).checked)
            {
                list.push($_course.get(4*i).innerHTML);
            }
        }
        if (list.length===0)
            return;
        let csrf = $("#csrf").val();
        $.post("", {"content":list, "csrfmiddlewaretoken":csrf},
            callback);
    }
)