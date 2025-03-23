#include "nwpwin.h"
#include "res.h"

class edit : public vsite::nwp::window {
protected:
	std::string class_name() override { return "edit"; }
};

class button : public vsite::nwp::window {
protected:
	std::string class_name() override { return "button"; }
};

class list_box : public vsite::nwp::window {
protected:
	std::string class_name() override { return "listbox"; }
};

class main_window : public vsite::nwp::window
{
	edit ed;
	button btn_a, btn_r;
	list_box listbox;
protected:
	int on_create(CREATESTRUCT* pcs) override;
	void on_command(int id) override;
	void on_destroy() override;
};

int main_window::on_create(CREATESTRUCT* pcs)
{
	listbox.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, IDC_LB, 40, 10, 200, 200);
	ed.create(*this, WS_CHILD | WS_VISIBLE | WS_BORDER, 0, IDC_EDIT, 250, 10, 300, 20);
	btn_a.create(*this, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, "Add", IDC_ADD, 300, 50, 100, 30);
	btn_r.create(*this, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, "Remove", IDC_REMOVE, 300, 80, 100, 30);
	EnableWindow(btn_r, FALSE);
	return 0;
}

void main_window::on_command(int id) {
	switch (id) {
	case ID_FILE_EXIT:
		on_destroy();
		break;
	case ID_HELP_ABOUT:
		MessageBox(*this, "add adds to list, remove removes from list", "Help", MB_ICONINFORMATION | MB_OK);
		break;
	case IDC_ADD:
		char str[64];
		GetDlgItemText(*this, IDC_EDIT, str, sizeof(str));
		SendDlgItemMessage(*this, IDC_LB, LB_ADDSTRING, 0, (LPARAM)str);
		EnableWindow(btn_r, TRUE);
		break;
	case IDC_REMOVE:
		int selection = SendMessage(listbox, LB_GETCURSEL, 0, 0);
		SendMessage(listbox, LB_DELETESTRING, selection, 0);

		int elems = SendMessage(listbox, LB_GETCOUNT, 0, 0);
		if (!elems) {
			EnableWindow(btn_r, FALSE);
		}
		break;
	}
}

void main_window::on_destroy() {
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	main_window w;
	w.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, "NWP 2", (int)::LoadMenu(instance, MAKEINTRESOURCE(IDM_V2)));
	vsite::nwp::set_icons(instance, w, IDI_V2);
	vsite::nwp::application app;
	return app.run();
}
