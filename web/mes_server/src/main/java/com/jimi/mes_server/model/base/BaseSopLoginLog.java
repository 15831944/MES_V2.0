package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseSopLoginLog<M extends BaseSopLoginLog<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setUserName(java.lang.String userName) {
		set("user_name", userName);
		return (M)this;
	}
	
	public java.lang.String getUserName() {
		return getStr("user_name");
	}

	public M setLogSiteNumber(java.lang.String logSiteNumber) {
		set("log_site_number", logSiteNumber);
		return (M)this;
	}
	
	public java.lang.String getLogSiteNumber() {
		return getStr("log_site_number");
	}

	public M setTime(java.util.Date time) {
		set("time", time);
		return (M)this;
	}
	
	public java.util.Date getTime() {
		return get("time");
	}

}
