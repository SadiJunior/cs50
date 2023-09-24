document.addEventListener("DOMContentLoaded", function()
{
    let greetingParagraph = document.createElement("p");
    greetingParagraph.setAttribute("id", "greetingParagraph");

    document.querySelector("body").appendChild(greetingParagraph);

    let input = document.querySelector("#name");

    input.addEventListener("keyup", function(event)
    {
        let paragraph = document.querySelector("#greetingParagraph");

        if (input.value)
        {
            paragraph.innerHTML = `hello, ${input.value}`;
        }
        else
        {
            paragraph.innerHTML = "hello, whoever you are";
        }
    });
});