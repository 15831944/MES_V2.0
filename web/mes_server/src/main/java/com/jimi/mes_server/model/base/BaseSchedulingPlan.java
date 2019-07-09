package com.jimi.mes_server.model.base;

import com.jfinal.plugin.activerecord.Model;
import com.jfinal.plugin.activerecord.IBean;

/**
 * Generated by JFinal, do not modify this file.
 */
@SuppressWarnings({"serial", "unchecked"})
public abstract class BaseSchedulingPlan<M extends BaseSchedulingPlan<M>> extends Model<M> implements IBean {

	public M setId(java.lang.Integer id) {
		set("id", id);
		return (M)this;
	}
	
	public java.lang.Integer getId() {
		return getInt("id");
	}

	public M setOrders(java.lang.Integer orders) {
		set("orders", orders);
		return (M)this;
	}
	
	public java.lang.Integer getOrders() {
		return getInt("orders");
	}

	public M setIsUrgent(java.lang.Boolean isUrgent) {
		set("is_urgent", isUrgent);
		return (M)this;
	}
	
	public java.lang.Boolean getIsUrgent() {
		return get("is_urgent");
	}

	public M setProcessGroup(java.lang.Integer processGroup) {
		set("process_group", processGroup);
		return (M)this;
	}
	
	public java.lang.Integer getProcessGroup() {
		return getInt("process_group");
	}

	public M setLine(java.lang.Integer line) {
		set("line", line);
		return (M)this;
	}
	
	public java.lang.Integer getLine() {
		return getInt("line");
	}

	public M setSchedulingQuantity(java.lang.Integer schedulingQuantity) {
		set("scheduling_quantity", schedulingQuantity);
		return (M)this;
	}
	
	public java.lang.Integer getSchedulingQuantity() {
		return getInt("scheduling_quantity");
	}

	public M setCapacity(java.lang.Integer capacity) {
		set("capacity", capacity);
		return (M)this;
	}
	
	public java.lang.Integer getCapacity() {
		return getInt("capacity");
	}

	public M setProductionPlanningNumber(java.lang.String productionPlanningNumber) {
		set("production_planning_number", productionPlanningNumber);
		return (M)this;
	}
	
	public java.lang.String getProductionPlanningNumber() {
		return getStr("production_planning_number");
	}

	public M setPlanStartTime(java.util.Date planStartTime) {
		set("plan_start_time", planStartTime);
		return (M)this;
	}
	
	public java.util.Date getPlanStartTime() {
		return get("plan_start_time");
	}

	public M setPlanCompleteTime(java.util.Date planCompleteTime) {
		set("plan_complete_time", planCompleteTime);
		return (M)this;
	}
	
	public java.util.Date getPlanCompleteTime() {
		return get("plan_complete_time");
	}

	public M setStartTime(java.util.Date startTime) {
		set("start_time", startTime);
		return (M)this;
	}
	
	public java.util.Date getStartTime() {
		return get("start_time");
	}

	public M setCompleteTime(java.util.Date completeTime) {
		set("complete_time", completeTime);
		return (M)this;
	}
	
	public java.util.Date getCompleteTime() {
		return get("complete_time");
	}

	public M setProducedQuantity(java.lang.Integer producedQuantity) {
		set("produced_quantity", producedQuantity);
		return (M)this;
	}
	
	public java.lang.Integer getProducedQuantity() {
		return getInt("produced_quantity");
	}

	public M setRemainingQuantity(java.lang.Integer remainingQuantity) {
		set("remaining_quantity", remainingQuantity);
		return (M)this;
	}
	
	public java.lang.Integer getRemainingQuantity() {
		return getInt("remaining_quantity");
	}

	public M setLineChangeTime(java.lang.String lineChangeTime) {
		set("line_change_time", lineChangeTime);
		return (M)this;
	}
	
	public java.lang.String getLineChangeTime() {
		return getStr("line_change_time");
	}

	public M setIsTimeout(java.lang.Boolean isTimeout) {
		set("is_timeout", isTimeout);
		return (M)this;
	}
	
	public java.lang.Boolean getIsTimeout() {
		return get("is_timeout");
	}

	public M setSchedulingPlanStatus(java.lang.Integer schedulingPlanStatus) {
		set("scheduling_plan_status", schedulingPlanStatus);
		return (M)this;
	}
	
	public java.lang.Integer getSchedulingPlanStatus() {
		return getInt("scheduling_plan_status");
	}

	public M setRemark(java.lang.String remark) {
		set("remark", remark);
		return (M)this;
	}
	
	public java.lang.String getRemark() {
		return getStr("remark");
	}

	public M setRemainingReason(java.lang.String remainingReason) {
		set("remaining_reason", remainingReason);
		return (M)this;
	}
	
	public java.lang.String getRemainingReason() {
		return getStr("remaining_reason");
	}

}
