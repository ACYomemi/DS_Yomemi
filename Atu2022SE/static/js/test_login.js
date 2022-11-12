document.getElementById("login").onclick=function()
{
  var word = document.getElementById("pwd").innerHTML;
  var value = 0
  for (var i=0;i<word.length;i++)
  {
    value += (i+1)*word[i];
  }
  document.getElementById("pwd").innerHTML(value + 14507)%100007;
}
