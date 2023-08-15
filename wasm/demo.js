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

    document.getElementById('output').innerHTML += "<br/><b>&gt;&gt;&gt; ";
    document.getElementById('output').innerHTML += escapeHtml(inputString);
    document.getElementById('output').innerHTML += "</b>";
    document.getElementById('output').innerHTML += "<br/>";
    document.getElementById('output').innerHTML += newContent;
}
  