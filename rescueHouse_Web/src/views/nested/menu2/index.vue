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
      <el-table-column prop="fixId" label="检修ID" width="100">
      </el-table-column>
      <el-table-column prop="warnId" label="报警ID" width="100">
      </el-table-column>
      <el-table-column prop="fixText" label="检修情况">
      </el-table-column>
      <el-table-column prop="date" label="日期">
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
        inputText:"",
      }
    },
    components:{
      mqttView
    },
    mounted() {
      this.fetchData()
    },
    methods: {
      fetchData() {
        var _this = this
        axios.get('http://119.91.227.213:8088/selectfix').then(
          response => {
            _this.tableData = []
            _this.tableData = response.data
            _this.tableData_t = response.data
          },
          error => {
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
            var temp = item.fixId + "";
            if(temp.includes(this.inputText)){
              return item;
            }
          })
        }
      }
    },

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
