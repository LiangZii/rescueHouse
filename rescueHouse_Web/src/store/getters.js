const getters = {
  sidebar: state => state.app.sidebar,
  device: state => state.app.device,
  token: state => state.user.token,
  avatar: state => state.user.avatar,
  name: state => state.user.name,
  mqttreceiveNews: state => state.emqx.receiveNews,
  mqttConnected: state => state.emqx.client.connected,
}
export default getters
