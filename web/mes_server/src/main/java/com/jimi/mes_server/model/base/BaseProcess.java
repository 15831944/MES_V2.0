package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseProcess<M extends BaseProcess<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setProcessNo(java.lang.String processNo) {
		set("process_no", processNo);
		return (M)this;
	}
	
	public java.lang.String getProcessNo() {
		return getStr("process_no");
	}

	public M setProcessName(java.lang.String processName) {
		set("process_name", processName);
		return (M)this;
	}
	
	public java.lang.String getProcessName() {
		return getStr("process_name");
	}

	public M setProcessRemark(java.lang.String processRemark) {
		set("process_remark", processRemark);
		return (M)this;
	}
	
	public java.lang.String getProcessRemark() {
		return getStr("process_remark");
	}

	public M setProcessGroup(java.lang.Integer processGroup) {
		set("process_group", processGroup);
		return (M)this;
	}
	
	public java.lang.Integer getProcessGroup() {
		return getInt("process_group");
	}

}
