package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseProcessGroup<M extends BaseProcessGroup<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setGroupNo(java.lang.String groupNo) {
		set("group_no", groupNo);
		return (M)this;
	}
	
	public java.lang.String getGroupNo() {
		return getStr("group_no");
	}

	public M setGroupName(java.lang.String groupName) {
		set("group_name", groupName);
		return (M)this;
	}
	
	public java.lang.String getGroupName() {
		return getStr("group_name");
	}

	public M setGroupRemark(java.lang.String groupRemark) {
		set("group_remark", groupRemark);
		return (M)this;
	}
	
	public java.lang.String getGroupRemark() {
		return getStr("group_remark");
	}

	public M setPosition(java.lang.Integer position) {
		set("position", position);
		return (M)this;
	}
	
	public java.lang.Integer getPosition() {
		return getInt("position");
	}

	public M setFactory(java.lang.Integer factory) {
		set("factory", factory);
		return (M)this;
	}
	
	public java.lang.Integer getFactory() {
		return getInt("factory");
	}

}
