// Load the monaco editor


require.config({ paths: { vs: '../node_modules/monaco-editor/min/vs' } });
require(['vs/editor/editor.main'], function () {
	monaco.languages.register({
		id: 'myCustomLanguage'
	});
	monaco.languages.setMonarchTokensProvider('myCustomLanguage', {
		tokenizer: {
			root: [
				[/\[error.*/, 'custom-error'],
				[/\[notice.*/, 'custom-notice'],
				[/\[info.*/, 'custom-info'],
				[/\[[a-zA-Z 0-9:]+\]/, 'custom-date']
			]
		}
	});
	// Define a new theme that constains only rules that match this language
	monaco.editor.defineTheme('myCoolTheme', {
		colors: {},
		base: 'vs',
		inherit: false,
		rules: [
			{ token: 'custom-info', foreground: '808080' },
			{ token: 'custom-error', foreground: 'ff0000', fontStyle: 'bold' },
			{ token: 'custom-notice', foreground: 'FFA500' },
			{ token: 'custom-date', foreground: '008800' }
		]
	});
	var editor = monaco.editor.create(document.getElementById('container'), {
		theme: 'myCoolTheme',
		value: getCode(),
		language: 'myCustomLanguage'
	});
});
function getCode() {
	return [ // starting code
		'cylinder(d1=50,d2=0,h=10,$fn=1);',
		''
	].join('\n');
}

function getEditorContent() {
	return monaco.editor.getModels()[0].getValue();
}
window.getEditorContent = getEditorContent;