const path = require('path');
const vscode = require('vscode');
const { LanguageClient } = require('vscode-languageclient/node');

let client;

function activate(context) {
    // Resolve the path to the server binary inside the extension folder
    const serverExe = path.join(context.extensionPath, 'build', 'LynxLSPServer');

    // Define server options
    let serverOptions = {
        run: { command: serverExe },
        debug: { command: serverExe, args: ['--debug'] }
    };

    // Define client options
    let clientOptions = {
        documentSelector: [{ scheme: 'file', language: 'lynx' }],
        synchronize: {
            fileEvents: vscode.workspace.createFileSystemWatcher('**/*.lynx')
        }
    };

    // Create the language client
    client = new LanguageClient(
        'LynxLSPServer',
        'Lynx Language Server',
        serverOptions,
        clientOptions
    );

    // Optional log when server is ready
    client.onDidChangeState((event) => {
        if (event.newState === 2) { // Running
            console.log('✅ Lynx LSP started.');
        }
    });

    // Start the client
    client.start();
}

function deactivate() {
    if (!client) return undefined;
    return client.stop();
}

module.exports = { activate, deactivate };
