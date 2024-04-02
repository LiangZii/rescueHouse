<template>
  <div class="dashboard-container">
    <!-- <div class="dashboard-text">name: {{ name }}</div> -->
    <!-- 卡片 -->
    <mqttView></mqttView>
    <el-row :gutter="40" class="panel-group">
      <el-col :xs="12" :sm="12" :lg="10" class="card-panel-col">
        <div class="card-panel">
          <div class="card-panel-col-left">
            <div class="card-panel-icon-wrapper icon-rescue">
              <svg-icon icon-class="rescue-house" class-name="card-panel-icon" />
            </div>
            <div class="card-panel-description">
              <div class="card-panel-text">救援仓总数</div>
              <span class="card-panel-num">{{rescueAll}}</span>
            </div>
          </div>
          <div class="card-panel-col-right">
            <div class="card-panel-icon-wrapper icon-yunxing">
              <svg-icon icon-class="yunxing" class-name="card-panel-icon" />
            </div>
            <div class="card-panel-description">
              <div class="card-panel-text">正在使用数</div>
              <span class="card-panel-num">{{rescueUse}}</span>
            </div>
          </div>
        </div>
      </el-col>


      <el-col :xs="12" :sm="12" :lg="6" class="card-panel-col">
        <div class="card-panel" @click="handleSetLineChartData('messages')">
          <div class="card-panel-icon-wrapper icon-message">
            <svg-icon icon-class="message" class-name="card-panel-icon" />
          </div>
          <div class="card-panel-description">
            <div class="card-panel-text">未处理预警数</div>
            <span class="card-panel-num">{{warnAll}}</span>
          </div>
        </div>
      </el-col>
    </el-row>

    <el-row>
          <el-col :span="24">
            <line-echarts id="lineEcharts" height="300px" ref="echarts"></line-echarts>
          </el-col>
        </el-row>
  </div>
</template>

<script>
import { mapGetters } from 'vuex'
import lineEcharts from './ECharts/lineEcharts.vue'
import axios from 'axios'
import mqttView from '../../components/mqttView/index.vue'

export default {
  name: 'Dashboard',
  data() {
    return{
      rescueAll: 1,
      rescueUse: 1,
      warnAll: 1,
    }
  },
  components:{
    lineEcharts,
    mqttView
  },
  computed: {
    ...mapGetters([
      'name'
    ])
  },
  mounted () {
      this.selfAdaption(),
      this.fetchData()
    },
  methods: {
    handleSetLineChartData(type) {
      this.$router.push({path:'/nested/table'})
    },
    // echart自适应
        selfAdaption () {
          let that = this
          setTimeout(() => {
            window.onresize = function () {
              if (that.$refs.echarts) {
                that.$refs.echarts.chart.resize()
              }
            }
          }, 10)
        },
        fetchData() {
          var _this = this
          axios.get('http://119.91.227.213:8088/selectallrescue').then(
            response => {
              _this.rescueAll = response.data[0].rescueall
              _this.rescueUse = response.data[0].rescueuse
              _this.warnAll = response.datap[0].warnall
            },
            error => {
              // this.$message({'请求失败，请刷新重试', 'warning'});
              this.$message({message: '请求失败，请刷新重试',type: 'warning'});
            }
          )

        },
  }

}
</script>

<style lang="scss" scoped>
.dashboard {
  &-container {
    margin: 30px;
  }
  &-text {
    font-size: 30px;
    line-height: 46px;
  }
}

.panel-group {
  margin-top: 18px;

  .card-panel-col {
    margin-bottom: 32px;
  }

  .card-panel-col-left{
    float: left;
  }

  .card-panel-col-right{
    float: right;
    margin-right: 60px;
  }

  .card-panel {
    height: 108px;
    cursor: pointer;
    font-size: 12px;
    position: relative;
    overflow: hidden;
    color: #666;
    background: #fff;
    box-shadow: 4px 4px 40px rgba(0, 0, 0, .05);
    border-color: rgba(0, 0, 0, .05);

    &:hover {

      .icon-message {
        background: #f4516c;
        color: #fff;
      }

    }

    .icon-message {
      color: #f4516c;
    }

    .icon-rescue {
      color: #34bfa3;
    }



    .card-panel-icon-wrapper {
      float: left;
      margin: 14px 0 0 14px;
      padding: 16px;
      transition: all 0.38s ease-out;
      border-radius: 6px;
    }

    .card-panel-icon {
      float: left;
      font-size: 48px;
    }

    .card-panel-description {
      float: left;
      font-weight: bold;
      margin: 26px;
      margin-left: 0px;

      .card-panel-text {
        line-height: 18px;
        color: rgba(0, 0, 0, 0.8);
        font-size: 16px;
        margin-bottom: 12px;
      }

      .card-panel-num {
        font-size: 20px;
        color: rgba(0, 0, 0, 0.45);
      }

    }
  }
}

@media (max-width:550px) {
  .card-panel-description {
    display: none;
  }

  .card-panel-icon-wrapper {
    float: none !important;
    width: 100%;
    height: 100%;
    margin: 0 !important;

    .svg-icon {
      display: block;
      margin: 14px auto !important;
      float: none !important;
    }
  }
}

</style>
