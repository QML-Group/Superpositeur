var Module = {
    var session = new Module.SuperpositeurCLISession();
    onRuntimeInitialized: function() {
        document.getElementById('log').innerHTML = session.input("help");
    }
};