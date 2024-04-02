<template>
  <div class="app-container">
    <mqttView></mqttView>
    <!-- 搜索筛选 -->
    <!-- <div class="search-operation">
      <div class="search-text">搜索:</div>
      <el-input class="search-input" v-model="input" placeholder="请输入内容"></el-input>
      <el-button class="search-button" type="primary" icon="el-icon-search">搜索</el-button>
    </div> -->
    <el-form :inline="true" :model="formInline" class="user-search">
      <el-form-item label="搜索：">
        <el-input v-model="inputText" size="small" placeholder="请输入内容" style="width: 270px;" @input="refreshData()"></el-input>
      </el-form-item>

      <el-form-item>
        <el-button size="small" type="primary" icon="el-icon-search" @click="refreshData()">搜索</el-button>
      </el-form-item>
    </el-form>


    <el-table :data="tableData" style="width: 100%" stripe>
      <el-table-column type="expand">
        <template slot-scope="props">
          <el-form label-position="left" inline class="demo-table-expand">
            <el-form-item label="城市">
              <span>{{ props.row.city }}</span>
            </el-form-item>
            <el-form-item label="剩余电量">
              <span>{{ props.row.battery }}</span>
            </el-form-item>
            <el-form-item label="经度">
              <span>{{ props.row.longitude }}</span>
            </el-form-item>
            <el-form-item label="纬度">
              <span>{{ props.row.latitude }}</span>
            </el-form-item>
            <el-form-item label="温度">
              <span>{{ props.row.temperature }}</span>
            </el-form-item>
            <el-form-item label="湿度">
              <span>{{ props.row.humidity }}</span>
            </el-form-item>
            <el-form-item label="光照强度">
              <span>{{ props.row.light }}</span>
            </el-form-item>
            <el-form-item label="PM 2.5">
              <span>{{ props.row.pm }}</span>
            </el-form-item>
          </el-form>
        </template>
      </el-table-column>
      <el-table-column label="救援仓 ID" prop="rescueId">
      </el-table-column>
      <el-table-column label="地区" prop="city">
      </el-table-column>
      <el-table-column label="报警数" prop="warning">
      </el-table-column>
    </el-table>
  </div>
</template>

<script>
  // import {getList} from '@/api/table'
  import axios from 'axios'
  import mqttView from '../../components/mqttView/index.vue'

  export default {
    filters: {

    },
    components: {
      mqttView
    },
    data() {
      return {
        list: null,
        inputText:"",
        listLoading: true,
        tableData: [],
        tableData_t:[],

      }
    },
    mounted() {
      this.fetchData()
    },
    methods: {
      fetchData() {
        var _this = this
        axios.get('http://119.91.227.213:8088/selectnew').then(
          response => {
            _this.tableData = []
            _this.tableData = response.data
            _this.tableData_t = response.data
          },
          error => {
            // this.$message({'请求失败，请刷新重试', 'warning'});
            this.$message({message: '请求失败，请刷新重试',type: 'warning'});
          }
        )
      },
      refreshData(){
        // console.log("进入refresh")
        if(this.inputText == ''){
          this.tableData = this.tableData_t;
        }else{
          this.tableData = this.tableData.filter(item => {
            // if(item.rescueId == this.inputText){
            //   return item;
            // }
            var temp = item.rescueId + "";
            if(temp.includes(this.inputText)){
              return item;
            }
          })
        }
      }
    }
  }
</script>

<style>
  .demo-table-expand {
    font-size: 0;
  }

  .demo-table-expand label {
    width: 90px;
    color: #99a9bf;
  }

  .demo-table-expand .el-form-item {
    margin-right: 0;
    margin-bottom: 0;
    width: 50%;
  }
</style>
