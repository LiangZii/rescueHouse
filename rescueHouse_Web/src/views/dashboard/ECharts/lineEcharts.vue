<template>
    <div>
      <div :id="id" :style="{width: width, height: height}"></div>
    </div>
</template>

<script>
import * as echarts from "echarts"
// import axios from 'axios'
// import mock from '@/mock/index.js'

export default {
  name: "lineEcharts",
  props: {
    id: {
      type: String,
      default: "myChart"
    },
    width: {
      type: String,
      default: "100%"
    },
    height: {
      type: String,
      default: "100%"
    }
  },
  data () {
    return {
      chart: null,
      dataxx:[5000,5000,5000,5000,5000,5000,5000],
    }
  },
  created () {
    // axios.get('http://localhost:8080/xxx').then(
    //         response => {
    //           //console.log('请求成功了',response.data.data.data)
    //           this.dataxx=response.data.data.data
    //           //console.log('现在的数据是',this.dataxx)

    //           this.textxx=response.data.text
    //           //console.log('现在的数据是',this.textxx)
    //           //this.initChart()
    //         },
    //         error => {
    //           //console.log('请求失败了',error.message)
    //         }
    //       )

  },

   mounted () {
     this.initChart()
   },
  methods: {
    initChart () {
      this.chart = echarts.init(document.getElementById(this.id))


      //console.log('请求xx了',this.textxx)
      this.chart.setOption({
        title: {
          text: "一周内访问人数折线图"
        },
        tooltip: {
          trigger: "axis"
        },
        legend: {
          data: ["访问人数"]
        },
        grid: {
          left: "3%",
          right: "4%",
          bottom: "3%",
          containLabel: true
        },
        toolbox: {
          feature: {
            saveAsImage: {}
          }
        },
        xAxis: {
          type: "category",
          boundaryGap: false,
          data: ["周一", "周二", "周三", "周四", "周五", "周六", "周日"]
        },
        yAxis: {
          type: "value"
        },
        series: [
          {
            name: "访问人数",
            type: "line",
            stack: "总量",
            data: [6520, 6320, 5640, 4340, 3400, 7426, 7663]
          }
        ]
      })
    }
  },
}
</script>

<style scoped>

</style>
