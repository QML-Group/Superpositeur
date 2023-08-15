session = undefined;

var Module = {
    onRuntimeInitialized: function() {
        session = new Module["SuperpositeurCLISession"]();
    }
};

function escapeHtml(unsafe)
{
    return unsafe
        .replace(/&/g, "&amp;")
        .replace(/</g, "&lt;")
        .replace(/>/g, "&gt;")
        .replace(/"/g, "&quot;")
        .replace(/'/g, "&#039;");
}

function input(inputString) {
    if (session == undefined) {
        return;
    }

    newContent = session.input(inputString);

    newContent = escapeHtml(newContent);
    newContent = newContent.replace(/(?:\r\n|\r|\n)/g, '<br/>');

    output = document.getElementById('output')
    
    toAdd = "<br/><b>&gt;&gt;&gt; ";
    toAdd += escapeHtml(inputString);
    toAdd += "</b>";
    toAdd += "<br/>";
    output.innerHTML += toAdd;
    output.innerHTML += newContent;
    output.scrollTop = output.scrollHeight
}
  