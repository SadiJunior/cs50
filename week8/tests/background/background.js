document.addEventListener("DOMContentLoaded", function()
{
    let body = document.querySelector("body");

    document.querySelector("#redButton").addEventListener("click", function()
    {
        body.style.backgroundColor = "red";
    });

    document.querySelector("#greenButton").addEventListener("click", function()
    {
        body.style.backgroundColor = "green";
    });

    document.querySelector("#blueButton").addEventListener("click", function()
    {
        body.style.backgroundColor = "blue";
    });
});