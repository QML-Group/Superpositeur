var Module = {
    onRuntimeInitialized: function() {
        session = new Module["SuperpositeurCLISession"]();
        document.getElementById('output').innerHTML = session.input("help");
    }
};