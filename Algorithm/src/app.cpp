#include "wx/wx.h"
#include <wx/listctrl.h>
#include <wx/simplebook.h>
#include <wx/spinctrl.h>
#include <wx/font.h>
#include <string>
#include "headers/entry.h"
#include "headers/encryption.h"
#include "headers/hash.h"
#include "headers/store.h";

using std::string;

class MyFrame : public wxFrame
{
public:
    MyFrame();

private:
    void ChangePage(wxShowEffect);
    void AddRow(string select, string ID, string org, string email, string user, string pass);
    void StoreRow();

    wxTextCtrl* loginInput;
    wxSimplebook* m_simplebook;
    wxStaticText* m_pageIndicator;
    wxListView* basicListView;
    vector<vector<int>> keys;

    wxTextCtrl* orgText;
    wxTextCtrl* emailText;
    wxTextCtrl* userText;
    wxTextCtrl* passText;

};

MyFrame::MyFrame() :wxFrame(NULL, wxID_ANY, "Valut v0.1", wxDefaultPosition, wxSize(700, 500)) {

    // create pages
    wxPanel* bg = new wxPanel(this, wxID_ANY);
    m_simplebook = new wxSimplebook(bg, wxID_ANY);
    m_simplebook->SetEffectTimeout(200);
    wxPanel* page1 = new wxPanel(m_simplebook, wxID_ANY);
    wxPanel* page2 = new wxPanel(m_simplebook, wxID_ANY);
    page1->SetBackgroundColour(wxColour(108, 107, 107));
    page2->SetBackgroundColour(wxColour(108, 107, 107));
    m_simplebook->AddPage(page1, wxString());
    m_simplebook->AddPage(page2, wxString());
    wxFont myFont(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_MEDIUM);
    page1->SetFont(myFont);
    page2->SetFont(myFont);

    // login page
    wxStaticText* loginLabel = new wxStaticText(page1, wxID_ANY, "Enter Password");
    loginInput = new wxTextCtrl(page1, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    wxButton* loginSubmit = new wxButton(page1, wxID_ANY, "Test Button", wxDefaultPosition, wxDefaultSize);

    // credentials page
    basicListView = new wxListView(page2);
    basicListView->AppendColumn("");
    basicListView->AppendColumn("ID");
    basicListView->AppendColumn("Organization");
    basicListView->AppendColumn("Email");
    basicListView->AppendColumn("Username");
    basicListView->AppendColumn("Password");
    basicListView->SetColumnWidth(0, 50);
    basicListView->SetColumnWidth(1, 50);
    basicListView->SetColumnWidth(2, 200);
    basicListView->SetColumnWidth(3, 200);
    basicListView->SetColumnWidth(4, 90);
    basicListView->SetColumnWidth(5, 90);
    wxButton* addEntry = new wxButton(page2, wxID_ANY, "Add", wxDefaultPosition, wxDefaultSize);
    orgText = new wxTextCtrl(page2, wxID_ANY, "org", wxDefaultPosition, wxDefaultSize);
    emailText = new wxTextCtrl(page2, wxID_ANY, "email", wxDefaultPosition, wxDefaultSize);
    userText = new wxTextCtrl(page2, wxID_ANY, "user", wxDefaultPosition, wxDefaultSize);
    passText = new wxTextCtrl(page2, wxID_ANY, "pass", wxDefaultPosition, wxDefaultSize);
    wxButton* deleteEntry = new wxButton(page2, wxID_ANY, "Delete Selected", wxDefaultPosition, wxDefaultSize);
    basicListView->SetBackgroundColour(wxColour(108, 107, 107));
    basicListView->SetForegroundColour(wxColour(245, 245, 245));


    // layouts
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(m_simplebook, wxSizerFlags(1).Expand());

    wxBoxSizer* labelSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* page1Sizer = new wxBoxSizer(wxVERTICAL);
    labelSizer->Add(loginLabel, wxSizerFlags(1).Border(wxBOTTOM).Bottom());
    inputSizer->Add(loginInput, wxSizerFlags().Top());
    inputSizer->Add(loginSubmit, wxSizerFlags().Top());
    page1Sizer->Add(labelSizer, wxSizerFlags(1).Center());
    page1Sizer->Add(inputSizer, wxSizerFlags(1).Center());
    page1->SetSizer(page1Sizer);

    wxBoxSizer* tableSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* delSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* addSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* page2Sizer = new wxBoxSizer(wxVERTICAL);
    tableSizer->Add(basicListView, wxSizerFlags().Bottom());
    delSizer->Add(deleteEntry, wxSizerFlags().Top());
    addSizer->Add(addEntry, wxSizerFlags().Center());
    addSizer->Add(orgText, wxSizerFlags().Center());
    addSizer->Add(emailText, wxSizerFlags().Center());
    addSizer->Add(userText, wxSizerFlags().Center());
    addSizer->Add(passText, wxSizerFlags().Center());
    page2Sizer->Add(tableSizer, wxSizerFlags(4).Center());
    page2Sizer->Add(delSizer, wxSizerFlags(1).Center());
    page2Sizer->Add(addSizer, wxSizerFlags(2).Center());
    page2->SetSizerAndFit(page2Sizer);

    bg->SetSizer(mainSizer);
    Layout();

    // populateListView
    /*
    basicListView->InsertItem(0, "");
    basicListView->SetItem(0, 1, "1");
    basicListView->SetItem(0, 2, "Google");
    basicListView->SetItem(0, 3, "Alphabet@gmail.com");
    basicListView->SetItem(0, 4, "Alphabet");
    basicListView->SetItem(0, 5, "Password");

    basicListView->InsertItem(0, "");
    basicListView->SetItem(0, 1, "2");
    basicListView->SetItem(0, 2, "Yahoo");
    basicListView->SetItem(0, 3, "Alphabet@gmail.com");
    basicListView->SetItem(0, 4, "Alphabet");
    basicListView->SetItem(0, 5, "Password");
    */
    CreateStatusBar();

    // event handle buttons
    loginSubmit->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent&) {ChangePage(wxSHOW_EFFECT_SLIDE_TO_TOP); });

    addEntry->Bind(wxEVT_BUTTON,
        [this](wxCommandEvent&) {StoreRow(); });

}

void MyFrame::ChangePage(wxShowEffect effect)
{
    string input = loginInput->GetValue().ToStdString();
    if (input == "" || input.length() > 54) wxLogStatus("Invalid Password");
    else {
        createDB();
        createTables();
        string username = selectUser();
        input = convert2hash(input);
        if (username == "") {
            insertUser(input);
            keys = key_gen(text2hexa(input));
            m_simplebook->SetEffect(effect);
            m_simplebook->AdvanceSelection(effect == wxSHOW_EFFECT_SLIDE_TO_TOP);
            wxLogStatus("");
        }
        else {
            if (username == input) {
                keys = key_gen(text2hexa(input));
                // grab data
                // decrypt and display it to table
                m_simplebook->SetEffect(effect);
                m_simplebook->AdvanceSelection(effect == wxSHOW_EFFECT_SLIDE_TO_TOP);
                wxLogStatus("");
            }
            else {
                wxLogStatus("Incorrect Password");
            }
        }
    }
}

void MyFrame::StoreRow() {
    string organization = orgText->GetValue().ToStdString();
    string email = emailText->GetValue().ToStdString();
    string username = userText->GetValue().ToStdString();
    string password = passText->GetValue().ToStdString();
    
    if (organization == "" && email == "" && username == "" && password == "")  wxLogStatus("Invalid Addition");
    else {
        organization = encrypt(organization, keys);
        email = encrypt(email, keys);
        username = encrypt(username, keys);
        password = encrypt(password, keys);
        insertData(organization, email, username, password);
        orgText->Clear();
        emailText->Clear();
        userText->Clear();
        passText->Clear();
        vector<string> credentials = selectCred();
        credentials.at(1) = decrypt(credentials.at(1), keys);
        credentials.at(2) = decrypt(credentials.at(2), keys);
        credentials.at(3) = decrypt(credentials.at(3), keys);
        credentials.at(4) = decrypt(credentials.at(4), keys);
        AddRow("unselected", credentials.at(0), credentials.at(1), credentials.at(2), credentials.at(3), credentials.at(4));
        wxLogStatus("Information Added");
    }



}

void MyFrame::AddRow(string select="unselected", string ID="", string org="", string email="", string user="", string pass="") {
    basicListView->InsertItem(0, select);
    basicListView->SetItem(0, 1, ID);
    basicListView->SetItem(0, 2, org);
    basicListView->SetItem(0, 3, email);
    basicListView->SetItem(0, 4, user);
    basicListView->SetItem(0, 5, pass);
}


class MyApp : public wxApp
{
public:
    virtual bool OnInit()
    {
        MyFrame* frame = new MyFrame();
        frame->Center();
        frame->Show();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);
