import mqtt from 'mqtt'

const state = {
  connection: {
    protocol: 'ws',
    host: '119.91.227.213',
    // ws: 8083; wss: 8084
    port: 8083,
    endpoint: '/mqtt',
    // for more options, please refer to https://github.com/mqttjs/MQTT.js#mqttclientstreambuilder-options
    clean: true,
    connectTimeout: 30 * 1000, // ms
    reconnectPeriod: 4000, // ms
    clientId:
      'emqx_vue_' + Math.random().toString(16).substring(2, 8),
    // auth   没有验证
    // username: 'emqx_test',
    // password: 'emqx_test',
  },
  subscription: {
    topic: 'publish_topic',
    qos: 0,
  },
  publish: {
    topic: 'subscribe_topic',
    qos: 0,
    payload: '{ "led2switch": 0 }',
  },
  receiveNews: '{"light": 1111}',
  qosList: [0, 1, 2],
  client: {
    connected: false,
  },
  subscribeSuccess: false,
  connecting: false,
  retryTimes: 0,
}

const mutations = {
  initData(state) {
    state.client = {
      connected: false,
    }
    state.retryTimes = 0
    state.connecting = false
    state.subscribeSuccess = false
  },
  handleOnReConnect(state) {
    state.retryTimes += 1
    if (state.retryTimes > 5) {
      try {
        state.client.end()
        // this.initData()
        this.commit('emqx/initData')
        state.$message.error('Connection maxReconnectTimes limit, stop retry')
      } catch (error) {
        state.$message.error(error.toString())
      }
    }
  },
  createConnection(state) {
    try {
      state.connecting = true
      const { protocol, host, port, endpoint, ...options } = state.connection
      const connectUrl = `${protocol}://${host}:${port}${endpoint}`
      state.client = mqtt.connect(connectUrl, options)
      if (state.client.on) {
        state.client.on('connect', () => {
          state.connecting = false
          // state.connected = true
          console.log('Connection succeeded!')
        })
        // this.client.on('reconnect', this.handleOnReConnect)
        // state.client.on('reconnect', this.commit('emqx/handleOnReConnect'))
        state.client.on('error', error => {
          console.log('Connection failed', error)
        })
        state.client.on('message', (topic, message) => {
          // this.receiveNews = this.receiveNews.concat(message)
          // this.handleReceiveMessage(state,message);
          // this.commit('emqx/handleReceiveMessage',message);
          state.receiveNews = message
          console.log(`Received message ${message} from topic ${topic}`)
        })
      }
    } catch (error) {
      state.connecting = false
      console.log('mqtt.connect error', error)
    }
  },
  // subscribe topic
  // https://github.com/mqttjs/MQTT.js#mqttclientsubscribetopictopic-arraytopic-object-options-callback
  doSubscribe(state) {
    const { topic, qos } = state.subscription
    state.client.subscribe(topic, { qos }, (error, res) => {
      if (error) {
        console.log('Subscribe to topics error', error)
        return
      }
      state.subscribeSuccess = true
      console.log('Subscribe to topics res', res)
    })
  },
  // unsubscribe topic
  // https://github.com/mqttjs/MQTT.js#mqttclientunsubscribetopictopic-array-options-callback
  doUnSubscribe(state) {
    const { topic } = state.subscription
    state.client.unsubscribe(topic, error => {
      if (error) {
        console.log('Unsubscribe error', error)
      }
    })
  },
  // publish message
  // https://github.com/mqttjs/MQTT.js#mqttclientpublishtopic-message-options-callback
  doPublish(state,sta) {
    console.log("sta:", sta)
    state.publish.payload = sta
    const { topic, qos, payload } = state.publish
    state.client.publish(topic, payload, { qos }, error => {
      if (error) {
        console.log('Publish error', error)
      }
    })
  },
  // disconnect
  // https://github.com/mqttjs/MQTT.js#mqttclientendforce-options-callback
  destroyConnection(state) {
    if (state.client.connected) {
      try {
        state.client.end(false, () => {
          // this.initData()
          this.commit('emqx/initData')
          console.log('Successfully disconnected!')
        })
      } catch (error) {
        console.log('Disconnect failed', error.toString())
      }
    }
  },
  handleProtocolChange(state,value) {
    state.connection.port = value === 'wss' ? '8084' : '8083'
  },
  handleReceiveMessage(state,msg){
    var jsonObj = JSON.parse(msg);
    state.receiveNews = jsonObj.light
    if(jsonObj.warnId == 0){
      this.$parent.$notify({
                title: '警告',
                message: '灯光阈值自动报警',
                type: 'warning'
              });
    }else if(jsonObj.warnId == 1){
      this.$parent.$notify({
                title: '警告',
                message: '手动报警',
                type: 'warning'
              });
    }
  }
}

const actions = {

}

export default {
  namespaced: true,
  state,
  mutations,
  actions
}
