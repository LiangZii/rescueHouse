package com.example.first_spring.mapper;

import com.example.first_spring.entity.*;
import org.apache.ibatis.annotations.Select;
import org.apache.ibatis.annotations.Update;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface rescueMapper {

    @Select("select * from rescue_house ")
    public List<rescue> selectrescue();

    @Select("select * from rescue_house where date=\"2023-12-07\" ")
    public List<rescue> selectnew();

    @Select("select * from warn ")
    public List<warn> selectwarn();

    @Select("select * from fix ")
    public List<fix> selectfix();

    @Select("select rescue_id,temperature,humidity,light,pm,date from  rescue_house where rescue_id=#{rescue_id}")
    public List<env> selectenv(int rescue_id);

    @Update("UPDATE rescue_house SET light=light+10 where date=\"2023-12-07\" and rescue_id=1")
    public void add1();

    @Update("UPDATE rescue_house SET light=#{light} where date=\"2023-12-07\" and rescue_id=1")
    public void updateguang(double guang);

    @Update("update allrescue set warnall = (select count(warn.warn_id) from warn)")
    public void sumwarn();

    @Select("select * from allrescue")
    public List<allrescue> selectallrescue();




}
