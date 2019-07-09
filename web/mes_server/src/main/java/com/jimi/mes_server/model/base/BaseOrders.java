package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseOrders<M extends BaseOrders<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setZhidan(java.lang.String zhidan) {
		set("zhidan", zhidan);
		return (M)this;
	}
	
	public java.lang.String getZhidan() {
		return getStr("zhidan");
	}

	public M setAlias(java.lang.String alias) {
		set("alias", alias);
		return (M)this;
	}
	
	public java.lang.String getAlias() {
		return getStr("alias");
	}

	public M setSoftModel(java.lang.String softModel) {
		set("soft_model", softModel);
		return (M)this;
	}
	
	public java.lang.String getSoftModel() {
		return getStr("soft_model");
	}

	public M setVersion(java.lang.String version) {
		set("version", version);
		return (M)this;
	}
	
	public java.lang.String getVersion() {
		return getStr("version");
	}

	public M setProductNo(java.lang.String productNo) {
		set("product_no", productNo);
		return (M)this;
	}
	
	public java.lang.String getProductNo() {
		return getStr("product_no");
	}

	public M setCustomerNumber(java.lang.String customerNumber) {
		set("customer_number", customerNumber);
		return (M)this;
	}
	
	public java.lang.String getCustomerNumber() {
		return getStr("customer_number");
	}

	public M setCustomerName(java.lang.String customerName) {
		set("customer_name", customerName);
		return (M)this;
	}
	
	public java.lang.String getCustomerName() {
		return getStr("customer_name");
	}

	public M setCreateTime(java.util.Date createTime) {
		set("create_time", createTime);
		return (M)this;
	}
	
	public java.util.Date getCreateTime() {
		return get("create_time");
	}

	public M setQuantity(java.lang.Integer quantity) {
		set("quantity", quantity);
		return (M)this;
	}
	
	public java.lang.Integer getQuantity() {
		return getInt("quantity");
	}

	public M setDeliveryDate(java.util.Date deliveryDate) {
		set("delivery_date", deliveryDate);
		return (M)this;
	}
	
	public java.util.Date getDeliveryDate() {
		return get("delivery_date");
	}

	public M setRemark(java.lang.String remark) {
		set("remark", remark);
		return (M)this;
	}
	
	public java.lang.String getRemark() {
		return getStr("remark");
	}

	public M setOrdersStatus(java.lang.Integer ordersStatus) {
		set("orders_status", ordersStatus);
		return (M)this;
	}
	
	public java.lang.Integer getOrdersStatus() {
		return getInt("orders_status");
	}

	public M setDeleteReason(java.lang.String deleteReason) {
		set("delete_reason", deleteReason);
		return (M)this;
	}
	
	public java.lang.String getDeleteReason() {
		return getStr("delete_reason");
	}

}
