<template>
  <div>
    <div id="BarEchart" style="width: 380px;height: 300px"></div>
  </div>
</template>


<script>
  import * as echarts from 'echarts'


  export default {
    data() {
      return{
        cardData: [{
            temperature:'20.26',
            humidity:'25.6',
            light:'40.0',
            pm:'10.26',
            rescueId:'1'
          }],

      }
    },
    mounted() {
      this.initChart(this.cardData)
    },
    methods: {
      initChart(cardDatax) {
        var myChart = echarts.init(document.getElementById('BarEchart'))
        myChart.setOption({

                                      xAxis: {
                                        type: 'category',
                                        data: ['温度', '湿度', '光强', 'PM2.5']
                                      },
                                      yAxis: {
                                        type: 'value'
                                      },
                                      series: [{
                                        data: [
                                          cardDatax[0].temperature,
                                          cardDatax[0].humidity,
                                          cardDatax[0].light,
                                          cardDatax[0].pm
                                        ],
                                        type: 'bar'
                                      }]
                                })


      }
    }
  }
</script>

<style>
</style>
