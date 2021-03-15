// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "UTLogicButton.h"

// ---------------------------------------------------------------------------

DelphiUI::DelphiUI(TCanvas* target, TImageList* button_image_list)
	: canvas(target), button_image_list_(button_image_list) {
}

TLogicButton* DelphiUI::CreateAndOwnButton(int left, int top, int width,
	int height) {
	TLogicButton* btn = new TLogicButton(left, top, width, height);
	this->managed_buttons_.emplace_back(btn);
	return btn;
}

void DelphiUI::OnMouseMove(int mouse_x, int mouse_y) {
	for (auto widget : this->managed_buttons_) {
		if (mouse_x > widget->left_ && mouse_x < widget->left_ +
			widget->width_ && mouse_y > widget->top_ && mouse_y < widget->top_ +
			widget->height_) {
			if (widget->phase_ == 0) {
				widget->phase_ = 1;
			}
		}
		else if (widget->phase_ == 1) {
			widget->phase_ = 0;
		}
	}
}

void DelphiUI::OnMouseDown(int mouse_x, int mouse_y, int mouse_btn) {
	for (auto widget : this->managed_buttons_) {
		if (widget->phase_ == 1) {
			widget->phase_ = 2;
		}
	}
}

void DelphiUI::OnMouseUp(int mouse_x, int mouse_y, int mouse_btn) {
	for (auto widget : this->managed_buttons_) {
		if (widget->phase_ == 2) {
			widget->phase_ = 0;
			widget->triggerCallback();
		}
	}
}

void DelphiUI::Render() {
	for (auto widget : this->managed_buttons_) {
		widget->Draw(this->button_image_list_, this->canvas);
	}
}

// ---------------------------------------------------------------------------
TLogicButton::TLogicButton(int left, int top, int width, int height)
	: left_(left), top_(top), width_(width), height_(height), phase_(0) {
}

TLogicButton::TLogicButton() : left_(0), top_(0), width_(0), height_(0),
	phase_(0) {
}

TRect TLogicButton::GetRect() const {
	return TRect(left_, top_, left_ + width_, top_ + height_);
}


void TLogicButton::SeteazaFaza(int valoare) {
	this->phase_ = valoare; // 0=visible //1=selected //2=pressed
	switch (this->phase_) {
	case 0:
		FIndex = VisibleIndex;
		break;
	case 1:
		FIndex = SelectedIndex;
		break;
	case 2:
		FIndex = PressedIndex;
		break;
	}
}

void TLogicButton::Draw(TImageList *lista, TCanvas *UnCanvas) {
	lista->Draw(UnCanvas, this->left_, this->top_, FIndex, true);
}

void TLogicButton::triggerCallback() {
	// call the callback
	if (this->callback_) {
		this->callback_(0);
	}

}


// ---------------------------------------------------------------------------
#pragma package(smart_init)
