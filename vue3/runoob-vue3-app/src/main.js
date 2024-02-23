import { createApp } from 'vue'

import App from './App.vue'
import HelloRunoob from './components/HelloRunoob.vue'

const app = createApp(App)
app.component('hello-runoob', HelloRunoob) // 自定义标签
app.mount('#app')