<template>
  <div class="app-container">
    <mqttView></mqttView>
  <div class="line-one">
    <!-- 卡片概览 -->
    <el-card class="box-card" shadow="hover" style="height: 265px;">
      <!-- <div slot="header" class="clearfix"> -->
        <div style="font-weight: bold;">当前环境概览</div>
        <input v-model="inputRescueId" type="text" style="margin-top: 15px;width: 60px;" @change="handlerInputChange()"></input>
        <!-- <el-button style="float: right; padding: 3px 0" type="text">刷新</el-button> -->
        <!-- <el-upload
          class="upload-demo"
          action="http://127.0.0.1:5000/predict"
          :before-remove="beforeRemove"
          :multiple = false
          :limit="1"
          :file-list="fileList"
          style="float: right;">
          <el-button size="samll" type="text">上传</el-button>
        </el-upload> -->
        <div  style="float: right;  padding: 3px;">
            <el-upload
              name="image"
              action="http://127.0.0.1:5000/predict"
              list-type="picture-card"
              :headers="headers"
              :on-preview="handlePreview"
              :file-list="fileList"
              :on-success="handleSuccess"
              :on-remove="handleRemove"
              :on-change="UploadImage1">
              <i class="el-icon-plus"></i>
            </el-upload>
        <div style="margin-top: 15px" class="text item">预测的结果是：{{results}}</div>
      </div>
    <!-- </div> -->


      <!-- <div class="box-card-in"> -->
        <!-- <div class="box-card-in-text"> -->
          <div style="margin-top: 15px" class="text item">救援仓ID：{{cardData[0].rescueId}}</div>
          <div class="text item">温度：{{cardData[0].temperature}}</div>
          <div class="text item">湿度：{{cardData[0].humidity}}</div>
          <div class="text item">光强：{{cardData[0].light}}</div>
          <div class="text item">PM2.5：{{cardData[0].pm}}</div>
        <!-- </div> -->


      <!-- </div> -->
    </el-card>

    <BarEchart  ref="echarts1"></BarEchart>
  </div>



    <div class="custom-charts">
      <!-- <BarEchart id="BarEchart" height="300px" width="300px" ref="echarts"></BarEchart> -->
      <!-- <PieEchart id="BarEchart" height="300px" ></PieEchart> -->
      <LinesEchart ref="echarts2"></LinesEchart>
    </div>
    <!-- <PieEchart id="PieEchart" height="300px"></PieEchart> -->

  </div>
</template>

<script>
  import BarEchart from "./Echarts/BarChart.vue"
  import LinesEchart from "./Echarts/LinesChart.vue"
  import PieEchart from "./Echarts/PieChart.vue"
  import axios from 'axios'
  import mqttView from '../../components/mqttView/index.vue'

  export default {
    data() {
      return {
        cardData: [{
          temperature:'14.26',
          humidity:'25.6',
          light:'40.0',
          pm:'10.26',
          rescueId:'1'
        }],
        fileList: [],
        imgSrc:'',
        inputRescueId:'1',


        results:"请上传图片",
        headers:{ enctype: "multipart/form-data"}
      }
    },
    components: {
      BarEchart,
      PieEchart,
      LinesEchart,
      mqttView
    },
    created() {
      this.fetchData()
    },
    methods: {
      fetchData() {
        var _this = this
        axios.post('http://119.91.227.213:8088/selectenv',{rescueId:this.inputRescueId},{
            headers: {'Content-Type': 'application/x-www-form-urlencoded'}
          }).then(
          response => {

            console.log("请求成功",response.data);
            _this.cardData = response.data

            this.$refs.echarts1.initChart(this.cardData);

        var arr2=[[0,0,0],
                  [0,0,0],
                  [0,0,0],
                  [0,0,0]]


          for(var i=0;i<3;i++){
                arr2[0][i] = this.cardData[i].temperature;
                arr2[1][i] = this.cardData[i].humidity;                
                arr2[2][i] = this.cardData[i].light;                
                arr2[3][i] = this.cardData[i].pm;                

            }

        console.log("新的数组是：",arr2)
        this.$refs.echarts2.initChart(arr2);

          },
          error => {
            // this.$message({'请求失败，请刷新重试', 'warning'});
            this.$message({message: '请求失败，请刷新重试',type: 'warning'});
          }
        )
      },
      beforeRemove(file, fileList) {
              return this.$confirm(`确定移除 ${ file.name }？`);
      },
      handlerInputChange(){
        this.fetchData()

        
      },

      handleSuccess(response, file, fileList) {
        this.fileList.push(file)
        this.fileList=fileList
        
        console.log("得到的结果是：",response)
        this.results=response.predictions[0].label
    },

    }
  }
</script>

<style lang="scss" scoped>

  .text {
    font-size: 14px;
  }

  .item {
    margin-bottom: 18px;
    font-weight: bold;
  }

  .box-card {
    width: 480px;
  }

  .custom-charts {
    display: flex;
    justify-content: space-around;
    margin-top: -10px;
    
  }

  .line-one{
    display: flex;
    justify-content: space-around;
  }

  .box-card-in{
    display: flex;
    align-items: center;
    justify-content: space-between;
  }

</style>
