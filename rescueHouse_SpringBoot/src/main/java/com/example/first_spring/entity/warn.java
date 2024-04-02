package com.example.first_spring.entity;

public class warn {
    private int WarnId;
    private int RescueId;
    private String reason;
    private String date;

    public int getWarnId() {
        return WarnId;
    }

    public void setWarnId(int warnId) {
        WarnId = warnId;
    }

    public int getRescueId() {
        return RescueId;
    }

    public void setRescueId(int rescueId) {
        RescueId = rescueId;
    }

    public String getReason() {
        return reason;
    }

    public void setReason(String reason) {
        this.reason = reason;
    }

    public String getDate() {
        return date;
    }

    public void setDate(String date) {
        this.date = date;
    }
}
