var Module = {
    onRuntimeInitialized: function() {
        session = new Module["SuperpositeurCLISession"]();
        document.getElementById('log').innerHTML = session.input("help");
    }
};