<template>
  <div style="padding:30px;">
    <mqttView></mqttView>
    <!-- 搜索 -->
    <el-form :inline="true" :model="formInline" class="user-search">
      <el-form-item label="搜索：">
        <el-input v-model="inputText" size="small" placeholder="请输入内容" style="width: 270px;" @input="refreshData()"></el-input>
      </el-form-item>

      <el-form-item>
        <el-button size="small" type="primary" icon="el-icon-search" @click="refreshData()">搜索</el-button>
      </el-form-item>
    </el-form>

    <!-- 表格table -->
    <el-table :data="tableData" style="width: 100%" stripe>
      <el-table-column prop="warnId" label="报警ID" width="100">
      </el-table-column>
      <el-table-column prop="rescueId" label="救援仓ID" width="100">
      </el-table-column>
      <el-table-column prop="reason" label="报警原因" min-width="200">
      </el-table-column>
      <el-table-column prop="date" label="报警日期">
      </el-table-column>
      <el-table-column align="center" label="操作" >
              <template slot-scope="scope">
                <el-button size="mini" type="primary" @click="handleDo(scope.$index, scope.row)">处理</el-button>
              </template>
            </el-table-column>
    </el-table>
  </div>
</template>

<script>
  import axios from 'axios'
  import mqttView from '../../../components/mqttView/index.vue'

  export default {
    data() {
      return {
        tableData: [],
        tableData_t:[],
        inputText:""
      }
    },
    mounted() {
      this.fetchData()
    },
    components:{
      mqttView
    },
    methods: {
      fetchData() {
        var _this = this
        axios.get('http://119.91.227.213:8088/selectwarn').then(
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
            // if(item.warnId == this.inputText){
            //   return item;
            // }
            var temp = item.warnId + "";
            if(temp.includes(this.inputText)){
              return item;
            }
          })
        }
      },
      handleDo(index,row){
        this.tableData.splice(index,1);
      }
    }
  }
</script>

<style>
  .el-table .warning-row {
    background: oldlace;
  }

  .el-table .success-row {
    background: #f0f9eb;
  }
</style>
