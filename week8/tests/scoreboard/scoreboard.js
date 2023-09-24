document.addEventListener("DOMContentLoaded", function()
{
    let team1 = 0;
    let team2 = 0;

    let main = document.querySelector("main");

    let team1_button = document.querySelector("#team1-button");
    let team2_button = document.querySelector("#team2-button");

    let team1_scoreElement = document.querySelector("#team1-score");
    let team2_scoreElement = document.querySelector("#team2-score");

    team1_button.addEventListener("click", function(event)
    {
        team1 += 1;

        updateScoreAndCheckWinner(team1, team1_scoreElement, "Team 1 Won");
    });

    team2_button.addEventListener("click", function(event)
    {
        team2 += 1;

        updateScoreAndCheckWinner(team2, team2_scoreElement, "Team 2 Won");
    });

    function updateScoreAndCheckWinner(score, scoreElement, winnerMessage)
    {
        scoreElement.innerHTML = score;

        if (score >= 10)
        {
            finishGame(winnerMessage);
        }
    }

    function finishGame(winnerMessage)
    {
        team1_button.disabled = true;
        team2_button.disabled = true;

        addGameOverParagraph(winnerMessage);
        addRestartButton();
    }

    function addGameOverParagraph(winnerMessage)
    {
        let gameOverParagraph = document.createElement("p");
        gameOverParagraph.setAttribute("id", "gameOver-paragraph");
        gameOverParagraph.innerHTML = `<b>Game over!</b><br>${winnerMessage}`;

        main.appendChild(gameOverParagraph);
    }

    function removeGameOverParagraph()
    {
        document.querySelector("#gameOver-paragraph").remove();
    }

    function addRestartButton()
    {
        let restartButton = document.createElement("button");
        restartButton.setAttribute("id", "restart-button");
        restartButton.innerHTML = "Restart Game";

        restartButton.addEventListener("click", function(event)
        {
            restartGame();
        });

        main.appendChild(restartButton);
    }

    function removeRestartButton()
    {
        document.querySelector("#restart-button").remove();
    }

    function restartGame()
    {
        team1 = 0;
        team2 = 0;

        team1_button.disabled = false;
        team2_button.disabled = false;

        team1_scoreElement.innerHTML = team1;
        team2_scoreElement.innerHTML = team2;

        removeGameOverParagraph();
        removeRestartButton();
    }
});