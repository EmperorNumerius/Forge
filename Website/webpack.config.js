const MonacoWebpackPlugin = require('monaco-editor-webpack-plugin');

module.exports = {
  plugins: [
    new MonacoWebpackPlugin({
      languages: ['typescript', 'javascript', 'json'], // Specify the necessary languages
    }),
  ],
};
