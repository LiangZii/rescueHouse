<template>
  <div>
    <div :id="id" :style="{width: width, height: height}"></div>
  </div>
</template>


<script>
  import * as echarts from "echarts"

  export default {
    name: "PieEchart",
    props: {
      id: {
        type: String,
        default: "PieEchart"
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
    data() {

    },
    mounted() {
      this.initChart()
    },
    methods: {
      initChart() {
        var chartDom = document.getElementById(this.id);
        var myChart = echarts.init(chartDom);
        var option;

        option = {
          tooltip: {
            trigger: 'item'
          },
          legend: {
            top: '5%',
            left: 'center'
          },
          series: [
            {
              name: 'Access From',
              type: 'pie',
              radius: ['40%', '70%'],
              avoidLabelOverlap: false,
              label: {
                show: false,
                position: 'center'
              },
              emphasis: {
                label: {
                  show: true,
                  fontSize: 40,
                  fontWeight: 'bold'
                }
              },
              labelLine: {
                show: false
              },
              data: [
                { value: 1048, name: 'Search Engine' },
                { value: 735, name: 'Direct' },
                { value: 580, name: 'Email' },
                { value: 484, name: 'Union Ads' },
                { value: 300, name: 'Video Ads' }
              ]
            }
          ]
        };

        option && myChart.setOption(option);

      }
    }
  }
</script>

<style>


</style>
