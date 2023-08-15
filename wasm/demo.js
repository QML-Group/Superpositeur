session = undefined;

var Module = {
    onRuntimeInitialized: function() {
        session = new Module["SuperpositeurCLISession"]();
    }
};

function input(inputString) {
    if (session == undefined) {
        return;
    }

    newContent = session.input(inputString);

    newContent = newContent.replace(/(?:\r\n|\r|\n)/g, '<br>');

    document.getElementById('output').innerHTML += "<br> &gt;&gt;&gt; ";
    document.getElementById('output').innerHTML += inputString;
    document.getElementById('output').innerHTML += "<br>";
    document.getElementById('output').innerHTML += newContent;
}
  