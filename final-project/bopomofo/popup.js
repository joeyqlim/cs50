let keys = document.getElementsByClassName("key");
let textArea = document.querySelector("#textarea");
let message = document.querySelector("#message");

for (let key of keys) {
    key.addEventListener("click", print)
}

function print(e) {
    character = e.target.id;
    if (character !== "disabled") {
        textArea.value += character;
        textArea.select();
        document.execCommand("copy");
        message.innerHTML = `<span id="copied">Copied to clipboard!</span>`
        setTimeout(function() {message.innerHTML = ""}, 3000);
    }
}
