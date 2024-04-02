<template>
  <div class="app-container">
    <mqttView></mqttView>
    <baidu-map class='bm-view' @ready='baiduMapInit' style="height:600px">
       <bm-marker v-for="(item,index) in pathArr" :key="item.rescueId" :position="{lng: item.longitude, lat: item.latitude}"  @click="lookDetail(item)">
           </bm-marker>
           <bm-info-window :position="infoWindow.info" :show="infoWindow.show" @close="infoWindowClose" @open="infoWindowOpen" :offset="{width: -6, height: -12}"
>{{infoWindow.floatBox}}</bm-info-window>
    </baidu-map>
    <!-- <baidu-map :center="{lng:108.910752,lat:34.237372} " zoom="15" style="height:800px"></baidu-map> -->
  </div>
</template>

<script>
  import axios from 'axios'
  import mqttView from '../../components/mqttView/index.vue'

  export default {
    name: 'baiduMap-demo',
    data() {
      return {
        pathArr: [],
        infoWindow: {
                show: false,
                floatBox:'',
                info: {
                  lng: "",
                  lat: "",
                }
      }
    }
    },
    mounted() {
      // this.baiduMapInit()
      this.fetchData()
    },
    components: {
      mqttView
    },
    methods: {
      baiduMapInit({BMap,map}) {
        this.BMap = BMap
        this.map = map
        var point = new BMap.Point(116.3, 39.9)
        // var point = new BMap.Point(116.404, 39.915)
        map.centerAndZoom(point, 5)
        map.setCurrentCity('北京')
        map.enableScrollWheelZoom(true) //鼠标滚动缩放

      },
      lookDetail(item){
        console.log("city:"+ item.city +"   lng:" + item.longitude + "   lat:" + item.latitude)
        this.infoWindow.info.lng = item.longitude;
        this.infoWindow.info.lat = item.latitude;
        this.infoWindow.floatBox = item.city;
        this.infoWindow.show = true;
      },
      infoWindowClose () {
            this.infoWindow.show = false
      },
      infoWindowOpen () {
            this.infoWindow.show = true
      },
      fetchData() {
        var _this = this
        axios.get('http://119.91.227.213:8088/selectnew').then(
          response => {
            _this.pathArr = []
            _this.pathArr = response.data
          },
          error => {
            // this.$message({'请求失败，请刷新重试', 'warning'});
            this.$message({message: '请求失败，请刷新重试',type: 'warning'});
          }
        )
      }
    }

  }
</script>

<style>
  .app-container {
    width: 100%;
    height: 100%;
  }
</style>
