
AOS.init();

var typed = new Typed(
    ".auto-type", {
        strings: ["Software Developer.", "Student.", "Lover of Art.", "Creator."],
        startDelay: 2000,
        backDelay: 1000,
        typeSpeed: 150,
        backSpeed: 100,
        loop: true,
    });

const modal = document.getElementById("sugar-modal-image");
const openButton = document.getElementById("open-sugar-image");
const closeButton = document.getElementById("close-button");

openButton.addEventListener("click", function (event) {
        event.preventDefault();
        modal.style.display = "flex";
        disableScroll();
        disableTab();
        modal.focus();
    });

closeButton.addEventListener("click", function () {
        modal.style.display = "none";
        enableScroll();
        enableTab();
        openButton.focus();
    });

window.addEventListener("click", function (event) {
        if (event.target === modal) {
            modal.style.display = "none";
            enableScroll();
            enableTab();
            openButton.focus();
        }
    });

function enableScroll()   {
    document.body.style.overflow = "auto";
}

function disableScroll() {
    document.body.style.overflow = "hidden";
}

function enableTab() {
    document.removeEventListener("keydown", blockTab);
}

function disableTab() {
    document.addEventListener("keydown", blockTab);
}

function blockTab(event) {
    if (event.key === "Tab") {
        event.preventDefault();
    }
}
