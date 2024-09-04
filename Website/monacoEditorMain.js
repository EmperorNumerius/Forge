
require.config({ paths: { vs: 'https://unpkg.com/monaco-editor@latest/min/vs' } });
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
	monaco.editor.defineTheme('editorTheme', {
		colors: {
			'editor.background': '#151515',
		},
		base: 'vs-dark',
		inherit: true,
		rules: [
			{ token: 'custom-info', foreground: '808080' },
			{ token: 'custom-error', foreground: 'ff0000', fontStyle: 'bold' },
			{ token: 'custom-notice', foreground: 'FFA500' },
			{ token: 'custom-date', foreground: '008800' }
		]
	});
	var editor = monaco.editor.create(document.getElementById('container'), {
		theme: 'editorTheme',
		value: getCode(),
		language: 'c'
	});
});

function getCode() {
	return [ // starting code
		'cylinder(d1=5,d2=0,h=5,$fn=10);',
		''
	].join('\n');
}

function getEditorContent() {
	return monaco.editor.getModels()[0].getValue();
}
window.getEditorContent = getEditorContent;

export function setMonacoEditorContent(content) {
	monaco.editor.getModels()[0].setValue(content);
}