<template>

<div v-loading.fullscreen.lock="fullscreenLoading"></div>

</template>

<script>
  import { mapGetters } from 'vuex'
  import store from '@/store/index'
  import mqtt from 'mqtt'

  export default {
    name: 'mqttView',
    data () {
      return {
        fullscreenLoading: false,
      }
    },
    props: {

    },
    computed: {
      ...mapGetters([
        'mqttreceiveNews',
        'mqttConnected'
      ])
    },
    watch:{
      mqttreceiveNews(newVal,oldVal){
        this.handleReceiveMessage(newVal);
      },
    },
    methods: {
      handleReceiveMessage(msg){
        var jsonObj = JSON.parse(msg);
        // this.receiveNews = jsonObj.light
        if(jsonObj.warnId == 0){
          this.$notify({
                    title: '警告',
                    message: '灯光阈值自动报警',
                    type: 'warning'
                  });
        }else if(jsonObj.warnId == 1){
          this.$notify({
                    title: '警告',
                    message: '手动报警',
                    type: 'warning'
                  });
        }

      }
    },
    mounted() {
      if(this.mqttConnected == false){
        this.$store.commit('emqx/initData')
        this.$store.commit('emqx/createConnection')
        this.$store.commit('emqx/doSubscribe')
        this.fullscreenLoading = false;
      }

    },
    unmounted() {
      if(this.mqttConnected == true){
        this.$store.commit('emqx/doUnSubscribe')
        this.$store.commit('emqx/destroyConnection')
      }

    }
  }

</script>

<style>
</style>
