package com.example.first_spring.entity;

public class fix {
    private int FixId;
    private int WarnId;
    private String FixText;
    private String date;

    public int getFixId() {
        return FixId;
    }

    public void setFixId(int fixId) {
        FixId = fixId;
    }

    public int getWarnId() {
        return WarnId;
    }

    public void setWarnId(int warnId) {
        WarnId = warnId;
    }

    public String getFixText() {
        return FixText;
    }

    public void setFixText(String fixText) {
        FixText = fixText;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }
}
