// ---------------------------------------------------------------------------
#ifndef UTLogicButtonH
#define UTLogicButtonH
// ---------------------------------------------------------------------------
#include <Vcl.Graphics.hpp>
#include <Vcl.Controls.hpp>

#include <functional>
#include <vector>

class DelphiUI;
class TLogicButton;

class DelphiUI {
protected:
	TCanvas* canvas;
	TImageList* button_image_list_;

	std::vector<TLogicButton*> managed_buttons_;
public:
	DelphiUI(TCanvas* target, TImageList* button_image_list);
	~DelphiUI();

	/**
		Creates a button and assume ownership of the object
		(will destroy along with this instance of DelphiUI).
	*/
	TLogicButton* CreateAndOwnButton(int left, int top, int width, int height);

	/**
		Renders the UI
	*/
	void Render();

	/**
		Renders the UI on a custom target.
	*/
	void Render(TCanvas* target);

	/**
		Reacts to mouse move events.

		@param mouse_x X axis position of mouse down event.
		@param mouse_y Y axis position of mouse down event.
	*/
	void OnMouseMove(int mouse_x, int mouse_y);

	/**
		Reacts to mouse down events.

		@param mouse_x X axis position of mouse down event.
		@param mouse_y Y axis position of mouse down event.
		@param mouse_btn The mouse button of the event.
			0: left mouse button.
			1: right mouse button.
	*/
	void OnMouseDown(int mouse_x, int mouse_y, int mouse_btn);

	/**
		Reacts to mouse down events.

		@param mouse_x X axis position of mouse down event.
		@param mouse_y Y axis position of mouse down event.
		@param mouse_btn The mouse button of the event.
			0: left mouse button.
			1: right mouse button.
	*/
	void OnMouseUp(int mouse_x, int mouse_y, int mouse_btn);
};

class TLogicButton {
protected:
	int phase_image_idx_;

	void SetLeftPosition(int value);
	void SetTopPosition(int value);
	void SetWidth(int value);
	void SeteazaHeight(int valoare);

	std::function<void(int)> callback_;

public:
	int left_;
	int top_;
	int width_;
	int height_;
	int phase_;

	TLogicButton(int left, int top, int width, int height);
	TLogicButton();
	void setCallback(const std::function<void(int)> &callback){
		this->callback_ = callback;
	}

	void TriggerCallback();
	void SetPhase(int phase_num);

	TRect GetRect() const;

	int VisibleIndex;
	int SelectedIndex;
	int PressedIndex;

	void Draw(TImageList *lista, TCanvas *UnCanvas);
};
// ---------------------------------------------------------------------------
#endif
