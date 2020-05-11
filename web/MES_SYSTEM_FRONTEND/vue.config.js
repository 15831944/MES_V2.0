module.exports = {
  publicPath: '/mes_system/',
  productionSourceMap: false,
  runtimeCompiler: true,

  /*chainWebpack: config => {
    config.plugins.delete('prefetch');

/!*    config.optimization.splitChunks({
      chunks: 'all'
    })*!/

  }*/

  devServer: {
    proxy: {
      '/': {
        // target: 'http://183.236.111.164:6091/mock/5c1a00070a62aa1c2dde4ce4',
        target: 'http://10.10.11.90:8080/mes_server',

        changOrigin: true
      }
    },
  }
};
