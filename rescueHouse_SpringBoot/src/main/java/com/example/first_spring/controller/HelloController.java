package com.example.first_spring.controller;

import com.example.first_spring.entity.*;
import com.example.first_spring.mapper.rescueMapper;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@CrossOrigin
@RestController
public class HelloController
{
//    http://localhost:8080/hello
//    http://localhost:8080/hello?nickname=zhangsan&phone=123

    //    @PostMapping ("/posthello")
//    public String posthello(User user)
//    {
//        System.out.println(user);
//        return "你好";
//    }


    @Autowired
    private rescueMapper rMapper;

    @GetMapping("/hello")
    @Transactional
    public String hello(double guang)
    {
        rMapper.updateguang(guang);
        return "helloworld 光照强度是"+guang;
    }

    @GetMapping("/add1")
    //访问一次，光照加10
    public List add1()
    {
        rMapper.add1();
        List<rescue> list=rMapper.selectrescue();
        System.out.println(list);
        return list;
    }

    @GetMapping("/selectrescue")
    //访问一次，从数据库中获得所有数据
    public List selectrescue()
    {
        List<rescue> list=rMapper.selectrescue();
        System.out.println(list);
        return list;
    }

    @GetMapping("/selectnew")
    //访问一次，从数据库中获得最新的环境信息
    public List selectnew()
    {
        List<rescue> list=rMapper.selectnew();
        System.out.println(list);
        return list;
    }

    @GetMapping("/selectwarn")
    //访问一次，从数据库中获得报警信息
    public List selectwarn()
    {
        List<warn> list=rMapper.selectwarn();
        System.out.println(list);
        return list;
    }

    @GetMapping("/selectfix")
    public List selectfix()
    {
        List<fix> list=rMapper.selectfix();
        System.out.println(list);
        return list;
    }

    @PostMapping ("/selectenv")
    public List selectenv(int rescueId)
    {
        List<env> list=rMapper.selectenv(rescueId);
        System.out.println(list);
        return list;
    }

    @GetMapping ("/selectallrescue")
    public List selectallrescue()
    {
        rMapper.sumwarn();
        List<allrescue> list=rMapper.selectallrescue();
        System.out.println(list);
        return list;
    }




}
