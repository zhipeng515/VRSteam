function sendURL()
{
     var value = document.getElementById("aText").value.replace(/\s/g,"");
     if(value != "")
     {
        if(confirm("Go to " + value +"?"))
        {
            location.href = value;
        }
     }
     else
     {
        document.getElementById("warning").innerHTML = "Please Input Target";
     }
}
