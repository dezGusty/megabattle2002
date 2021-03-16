// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UTLogicButton.h"

// ---------------------------------------------------------------------------

DelphiUI::DelphiUI(TCanvas* target, TImageList* button_image_list)
	: canvas(target), button_image_list_(button_image_list) {
}

/**
	Creates a button and assume ownership of the object
	(will destroy along with this instance of DelphiUI).
*/
TLogicButton* DelphiUI::CreateAndOwnButton(int left, int top, int width,
	int height) {
	TLogicButton* btn = new TLogicButton(left, top, width, height);
	this->managed_buttons_.emplace_back(btn);
	return btn;
}

/**
	Reacts to mouse move events.

	@param mouse_x X axis position of mouse down event.
	@param mouse_y Y axis position of mouse down event.
*/
void DelphiUI::OnMouseMove(int mouse_x, int mouse_y) {
	for (auto widget : this->managed_buttons_) {
		if (mouse_x > widget->left_ && mouse_x < widget->left_ +
			widget->width_ && mouse_y > widget->top_ && mouse_y < widget->top_ +
			widget->height_) {
			if (widget->phase_ == 0) {
				widget->SetPhase(1);
			}
		}
		else if (widget->phase_ == 1) {
			widget->SetPhase(0);
		}
	}
}

/**
	Reacts to mouse down events.

	@param mouse_x X axis position of mouse down event.
	@param mouse_y Y axis position of mouse down event.
	@param mouse_btn The mouse button of the event.
		0: left mouse button.
		1: right mouse button.
*/
void DelphiUI::OnMouseDown(int mouse_x, int mouse_y, int mouse_btn) {
	for (auto widget : this->managed_buttons_) {
		if (widget->phase_ == 1) {
			widget->SetPhase(2);
		}
	}
}

/**
	Reacts to mouse down events.

	@param mouse_x X axis position of mouse down event.
	@param mouse_y Y axis position of mouse down event.
	@param mouse_btn The mouse button of the event.
		0: left mouse button.
		1: right mouse button.
*/
void DelphiUI::OnMouseUp(int mouse_x, int mouse_y, int mouse_btn) {
	for (auto widget : this->managed_buttons_) {
		if (widget->phase_ == 2) {
			widget->phase_ = 0;
			widget->TriggerCallback();
		}
	}
}

void DelphiUI::Render() {
	for (auto widget : this->managed_buttons_) {
		widget->Draw(this->button_image_list_, this->canvas);
	}
}

/**
	Renders the UI on a custom target.
*/
void DelphiUI::Render(TCanvas* target){
	for (auto widget : this->managed_buttons_) {
		widget->Draw(this->button_image_list_, target);
	}
}

// ---------------------------------------------------------------------------
TLogicButton::TLogicButton(int left, int top, int width, int height)
	: left_(left), top_(top), width_(width), height_(height), phase_(0), phase_image_idx_(0) {
}

TLogicButton::TLogicButton() : left_(0), top_(0), width_(0), height_(0),
	phase_(0), phase_image_idx_(0) {
}

TRect TLogicButton::GetRect() const {
	return TRect(left_, top_, left_ + width_, top_ + height_);
}


void TLogicButton::SetPhase(int phase_num) {
	this->phase_ = phase_num; // 0=visible //1=selected //2=pressed
	switch (this->phase_) {
	case 1:
		phase_image_idx_ = SelectedIndex;
		return;
	case 2:
		phase_image_idx_ = PressedIndex;
		return;
	case 0:
	default:
		phase_image_idx_ = VisibleIndex;
		return;
	}
}

void TLogicButton::Draw(TImageList *lista, TCanvas *UnCanvas) {
	lista->Draw(UnCanvas, this->left_, this->top_, this->phase_image_idx_, true);
}

void TLogicButton::TriggerCallback() {
	// call the callback
	if (this->callback_) {
		this->callback_(this->phase_);
	}

}


// ---------------------------------------------------------------------------
#pragma package(smart_init)
