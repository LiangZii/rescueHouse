<template>
  <div>
    <div id="LinesEchart" style="width: 1000px; height: 300px"></div>
  </div>
</template>


<script>
  import * as echarts from "echarts"

  export default {
    data() {
      return{
            datax:[[14.26, 15.34, 17.21],
                  [25.6, 24.23, 33.21],
                  [31, 53, 64],
                  [10.26, 7.86, 5.71]]
      }
    },
    mounted() {
      this.initChart(this.datax)
    },
    methods: {
      initChart(datax) {
        var myChart = echarts.init(document.getElementById("LinesEchart"))
        myChart.setOption({

          title: {
            text: '监测记录'
          },
          tooltip: {
            trigger: 'axis'
          },
          legend: {
            data: ['温度', '湿度', '光强', 'PM2.5']
          },
          grid: {
            left: '3%',
            right: '4%',
            bottom: '3%',
            containLabel: true
          },
          toolbox: {
            feature: {
              saveAsImage: {}
            }
          },
          xAxis: {
            type: 'category',
            boundaryGap: false,
            data: ['12-05', '12-06', '12-07']
          },
          yAxis: {
            type: 'value'
          },
          series: [
            {
              name: '温度',
              type: 'line',
              stack: 'Total',
              data: datax[0]
            },
            {
              name: '湿度',
              type: 'line',
              stack: 'Total',
              data:datax[1]
            },
            {
              name: '光强',
              type: 'line',
              stack: 'Total',
              data:datax[2]
            },
            {
              name: 'PM2.5',
              type: 'line',
              stack: 'Total',
              data: datax[3]
            }
          ]

        })


      }
    }
  }
</script>

<style>


</style>
