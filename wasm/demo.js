session = undefined;

var Module = {
    onRuntimeInitialized: function() {
        session = new Module["SuperpositeurCLISession"]();
        document.getElementById('output').innerHTML = session.input("help");
    }
};

function input(inputString) {
    if (session == undefined) {
        return;
    }

    newContent = session.input("help");

    document.getElementById('output').innerHTML += "<br> &gt;&gt;&gt; ";
    document.getElementById('output').innerHTML += inputString;
    document.getElementById('output').innerHTML += "<br>";
    document.getElementById('output').innerHTML += newContent;
}
  