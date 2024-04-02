<template>
  <div class="total">
    <mqttView></mqttView>

    <el-row :gutter="20">
      <el-col :span="6">
        <el-card class="box-card" shadow="hover">
          <div slot="header" class="clearfix">
            <span>光照度监控</span>
          </div>
          <div class="text_value">
            光照强度：{{light}} Lux
          </div>
          <div class="text_time">
            更新时间：{{nowTime}}
          </div>
        </el-card>
      </el-col>

      <el-col :span="6">
        <el-card class="box-card" shadow="hover">
          <div slot="header" class="clearfix">
            <span>灯2操控</span>
          </div>
          <div class="card_button">
            <el-button type="primary" @click="doPublish(1)">开灯</el-button>
            <el-button type="primary" @click="doPublish(0)">关灯</el-button>
          </div>

        </el-card>
      </el-col>

      <!-- <el-col :span="6"><div class="grid-content bg-purple"></div></el-col> -->
      <!-- <el-col :span="6"><div class="grid-content bg-purple"></div></el-col> -->
    </el-row>
  </div>
</template>

<script>
  import mqttView from '../../components/mqttView/index.vue'
  import {  mapGetters  } from 'vuex'
  import store from '@/store/index'

  export default {
    name: 'mqtt',
  computed: {
      ...mapGetters([
        'mqttreceiveNews',
        'mqttConnected'
      ])
    },
    data() {
      return {
        sendMessage:'',
        light:15,
        nowTime:''
      }
    },
    components: {
      mqttView
    },
    watch:{
      mqttreceiveNews(newVal,oldVal){
        this.handleReceiveMessage(newVal);
      },
    },
    mounted() {

    },
    methods: {
      doPublish(sta) {
        if (sta === 1) {
          this.sendMessage = '{ "led2switch": 1 }'
        } else {
          this.sendMessage = '{ "led2switch": 0 }'
        }
        this.$store.commit('emqx/doPublish',this.sendMessage)
      },
      handleReceiveMessage(msg){
        var jsonObj = JSON.parse(msg);
        // this.receiveNews = jsonObj.light
        if(jsonObj.light != null){
          this.light = jsonObj.light,
          this.getNowTime()
        }
      },
       // 获取当前的时间
      getNowTime() {
          let dt = new Date()
          var y = dt.getFullYear()
          var mt = (dt.getMonth() + 1).toString().padStart(2,'0')
          var day = dt.getDate().toString().padStart(2,'0')
          var h = dt.getHours().toString().padStart(2,'0')
          var m = dt.getMinutes().toString().padStart(2,'0')
          var s = dt.getSeconds().toString().padStart(2,'0')
          this.nowTime = y + "-" + mt + "-" + day + " " + h + ":" + m + ":" + s
      },

  },
  }
</script>

<style>

  .el-row {
    margin: 30px;
    &:last-child {
      margin-bottom: 0;
    }
  }
  .el-col {
    border-radius: 4px;
  }

  .total{
    padding-left: 20px;
  }

  .text_value{
    font-size: 18px;
    font-weight: bold;
    margin-bottom: 20px;
  }

  .text_time{
    font-size: 12px;
  }

  .card_button{
    display: flex;
    justify-content: space-around;
    margin-bottom: 15px;
  }

</style>
