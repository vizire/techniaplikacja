#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/menu.h>
#include <wx/choice.h>
#include <wx/notebook.h>
#include <wx/dialog.h>
#include <iostream>
#include <string>
#include <regex>
#include <map>
#include <set>
#include <chrono>
#include <fstream>

// global variables
float likeness = 1.0f;
float initialLikeness = 1.0f;
float likenessChange = 0.0f;

// IDs for events
const int ID_ANALYZE_TEXT = 1001;
const int ID_OPEN_FILE = 1002;
const int ID_SAVE_FILE = 1003;
const int ID_NEW_FILE = 1004;
const int ID_REVERT = 1005;
const int ID_SETTINGS = 1006;
const int ID_DOCUMENTATION = 1007;

// Enumeration for languages
enum class Language
{
    ENGLISH,
    SPANISH,
    FRENCH,
    POLISH  
};

// dialog for settings
class SettingsDialog : public wxDialog
{
public:
    SettingsDialog(wxWindow* parent)
        : wxDialog(parent, wxID_ANY, wxT("Settings"), wxDefaultPosition, wxSize(300, 300))
    {
        wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);
        wxNotebook* notebook = new wxNotebook(this, wxID_ANY);

        // creates the settings tab
        wxPanel* settingsPanel = new wxPanel(notebook, wxID_ANY);
        wxBoxSizer* settingsSizer = new wxBoxSizer(wxVERTICAL);
        
        capitalizeCheckBox = new wxCheckBox(settingsPanel, wxID_ANY, "Capitalize First Letter");
        spacingCheckBox = new wxCheckBox(settingsPanel, wxID_ANY, "Fix Spacing Around Punctuation");
        periodCheckBox = new wxCheckBox(settingsPanel, wxID_ANY, "Capitalize After Period");
        codingCheckBox = new wxCheckBox(settingsPanel, wxID_ANY, "Ignore Coding Terms");

        settingsSizer->Add(capitalizeCheckBox, 0, wxALL, 5);
        settingsSizer->Add(spacingCheckBox, 0, wxALL, 5);
        settingsSizer->Add(periodCheckBox, 0, wxALL, 5);
        settingsSizer->Add(codingCheckBox, 0, wxALL, 5); // ensures all checkboxes are added
        
        settingsPanel->SetSizer(settingsSizer);
        notebook->AddPage(settingsPanel, "Settings");

        // creates the languages tab
        wxPanel* languagesPanel = new wxPanel(notebook, wxID_ANY);
        wxBoxSizer* languagesSizer = new wxBoxSizer(wxVERTICAL);
        langEnglishRadio = new wxRadioButton(languagesPanel, wxID_ANY, "English", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
        langSpanishRadio = new wxRadioButton(languagesPanel, wxID_ANY, "Spanish");
        langFrenchRadio = new wxRadioButton(languagesPanel, wxID_ANY, "French");
        langPolishRadio = new wxRadioButton(languagesPanel, wxID_ANY, wxT("Polish"));

        languagesSizer->Add(langEnglishRadio, 0, wxALL, 5);
        languagesSizer->Add(langSpanishRadio, 0, wxALL, 5);
        languagesSizer->Add(langFrenchRadio, 0, wxALL, 5);
        languagesSizer->Add(langPolishRadio, 0, wxALL, 5);
        languagesPanel->SetSizer(languagesSizer);
        notebook->AddPage(languagesPanel, "Languages");

        dialogSizer->Add(notebook, 1, wxEXPAND | wxALL, 10);

        wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
        wxButton* okButton = new wxButton(this, wxID_OK, "OK");
        wxButton* cancelButton = new wxButton(this, wxID_CANCEL, "Cancel");

        buttonSizer->Add(okButton, 0, wxALL, 5);
        buttonSizer->Add(cancelButton, 0, wxALL, 5);

        dialogSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);

        SetSizerAndFit(dialogSizer);  // adjustes size to fit contents
    }

    wxCheckBox* codingCheckBox;
    wxCheckBox* capitalizeCheckBox;
    wxCheckBox* spacingCheckBox;
    wxCheckBox* periodCheckBox;
    wxRadioButton* langEnglishRadio;
    wxRadioButton* langSpanishRadio;
    wxRadioButton* langFrenchRadio;
    wxRadioButton* langPolishRadio;

    Language GetSelectedLanguage()
    {
        if (langEnglishRadio->GetValue())
            return Language::ENGLISH;
        else if (langSpanishRadio->GetValue())
            return Language::SPANISH;
        else if (langFrenchRadio->GetValue())
            return Language::FRENCH;
        else if (langPolishRadio->GetValue())
            return Language::POLISH;
        return Language::ENGLISH;
    }
};

// dialog for documentation
class DocumentationDialog : public wxDialog
{
public:
    DocumentationDialog(wxWindow* parent, Language lang)
        : wxDialog(parent, wxID_ANY, wxT("Documentation"), wxDefaultPosition, wxSize(1280, 900), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
    {
        wxBoxSizer* dialogSizer = new wxBoxSizer(wxVERTICAL);

        // creates the documentation text based on language
        wxString documentationText;

        // declares and initialize the documentation text
        switch (lang)
        {
        case Language::ENGLISH:
            documentationText =
                wxT("This program is a text analyzer. It allows you to:\n\n"
                    "1. Analyze text for capitalization and spacing issues.\n"
                    "2. Open, save, and edit text files.\n"
                    "3. Adjust settings to customize text analysis.\n"
                    "4. Explore language options for text analysis.\n\n"
                    "Use the 'Settings' option to adjust the analysis parameters, and "
                    "the 'Languages' tab to select the language preferences.\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n"
                    "!w polskim jest wiecej informacji\n");
            break;
        case Language::SPANISH:
            documentationText =
                wxT("Este programa es un analizador de texto. Te permite:\n\n"
                    "1. Analizar texto en busca de problemas de capitalización y espaciado.\n"
                    "2. Abrir, guardar y editar archivos de texto.\n"
                    "3. Ajustar configuraciones para personalizar el análisis de texto.\n"
                    "4. Explorar opciones de idioma para el análisis de texto.\n\n"
                    "Usa la opción 'Configuración' para ajustar los parámetros de análisis, y "
                    "la pestaña 'Idiomas' para seleccionar las preferencias de idioma.\n");
            break;
        case Language::FRENCH:
            documentationText =
                wxT("Ce programme est un analyseur de texte. Il vous permet de:\n\n"
                    "1. Analyser le texte pour les problèmes de capitalisation et d'espacement.\n"
                    "2. Ouvrir, enregistrer et éditer des fichiers texte.\n"
                    "3. Ajuster les paramètres pour personnaliser l'analyse du texte.\n"
                    "4. Explorer les options de langue pour l'analyse du texte.\n\n"
                    "Utilisez l'option 'Paramètres' pour ajuster les paramètres d'analyse, et "
                    "l'onglet 'Langues' pour sélectionner les préférences de langue.\n");
            break;
        case Language::POLISH:
            documentationText =
                wxT("Ten program to narzędzie do analizy tekstu. Pozwala na:\n\n"
                    "1. Analizowanie tekstu pod kątem problemów z kapitalizacją i odstępami.\n"
                    "2. Otwieranie, zapisywanie i edytowanie plików tekstowych.\n"
                    "3. Dostosowywanie ustawień w celu spersonalizowania analizy tekstu.\n"
                    "4. Eksplorowanie opcji językowych dla analizy tekstu.\n\n"
                    "Użyj opcji 'Ustawienia', aby dostosować parametry analizy, oraz "
                    "karty 'Języki', aby wybrać preferencje językowe.\n, ten program ma ukryta zmienna o nazwie likeness jest ona zmieniania zelezne od bledow\n"
                    "poczatkowo jest ona rowna 1.0 jednak za kazdym bledem jest zmieniana na gorsza oczywiscie kazdy bled ktory znajdzie jest naprawiany jesli w ustawieniach jest zaznaczone zeby go naprawic\n");
            break;
        }

        wxTextCtrl* docTextCtrl = new wxTextCtrl(this, wxID_ANY, documentationText,
            wxDefaultPosition, wxSize(1280, 900), wxTE_MULTILINE | wxTE_READONLY);

        dialogSizer->Add(docTextCtrl, 1, wxEXPAND | wxALL, 10);
        SetSizerAndFit(dialogSizer);

        // force the dialog to the full screen size
        SetSize(1280, 900);
        Center();
    }

private:
    void OnClose(wxCommandEvent& event)
    {
        EndModal(wxID_CLOSE);
    }
};

// main frame for the application
class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    void OnAnalyze(wxCommandEvent& event);
    void OnOpenFile(wxCommandEvent& event);
    void OnSaveFile(wxCommandEvent& event);
    void OnNewFile(wxCommandEvent& event);
    void OnRevert(wxCommandEvent& event);
    void OnShowSettings(wxCommandEvent& event);
    void OnShowDocumentation(wxCommandEvent& event);
    void UpdateUIBasedOnLanguage();

    wxTextCtrl* textCtrl;
    wxButton* analyzeButton;
    wxButton* openFileButton;
    wxButton* saveFileButton;
    wxButton* newFileButton;
    wxButton* revertButton;
    wxTextCtrl* resultLabel;
    wxScrolledWindow* scrolledWindow;

    wxDECLARE_EVENT_TABLE();

    std::string previousText;
    bool codingTerms = false;
    bool capitalizeFirstLetter = false;
    bool fixSpacing = false;
    bool capitalizeAfterPeriod = false;
    Language currentLanguage = Language::ENGLISH;
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_BUTTON(ID_ANALYZE_TEXT, MyFrame::OnAnalyze)
    EVT_BUTTON(ID_OPEN_FILE, MyFrame::OnOpenFile)
    EVT_BUTTON(ID_SAVE_FILE, MyFrame::OnSaveFile)
    EVT_BUTTON(ID_NEW_FILE, MyFrame::OnNewFile)
    EVT_BUTTON(ID_REVERT, MyFrame::OnRevert)
    EVT_MENU(ID_SETTINGS, MyFrame::OnShowSettings)
    EVT_MENU(ID_DOCUMENTATION, MyFrame::OnShowDocumentation)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(800, 500))
{
    wxPanel* panel = new wxPanel(this, wxID_ANY);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* leftSizer = new wxBoxSizer(wxVERTICAL);

    textCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxSize(400, 600), wxTE_MULTILINE);
    analyzeButton = new wxButton(panel, ID_ANALYZE_TEXT, "Analyze Text");
    openFileButton = new wxButton(panel, ID_OPEN_FILE, "Open File");
    saveFileButton = new wxButton(panel, ID_SAVE_FILE, "Save File");
    newFileButton = new wxButton(panel, ID_NEW_FILE, "New File");
    revertButton = new wxButton(panel, ID_REVERT, "Revert Changes");

    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* menu = new wxMenu;
    menu->Append(ID_SETTINGS, "Settings");
    menu->Append(ID_DOCUMENTATION, "Documentation");
    menuBar->Append(menu, "Options");
    SetMenuBar(menuBar);

    scrolledWindow = new wxScrolledWindow(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    scrolledWindow->SetScrollRate(5, 5);
    resultLabel = new wxTextCtrl(scrolledWindow, wxID_ANY, "", wxDefaultPosition, wxSize(400, 600), wxTE_MULTILINE | wxTE_READONLY);

    wxBoxSizer* resultSizer = new wxBoxSizer(wxVERTICAL);
    resultSizer->Add(resultLabel, 1, wxEXPAND | wxALL, 10);
    scrolledWindow->SetSizer(resultSizer);

    leftSizer->Add(textCtrl, 1, wxEXPAND | wxALL, 10);
    leftSizer->Add(analyzeButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    leftSizer->Add(openFileButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    leftSizer->Add(saveFileButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    leftSizer->Add(newFileButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    leftSizer->Add(revertButton, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
    leftSizer->Add(scrolledWindow, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(leftSizer);

    // initial update based on default language and settings
    UpdateUIBasedOnLanguage();
    capitalizeFirstLetter = false; 
    fixSpacing = false;            
    capitalizeAfterPeriod = false;
    codingTerms = false;
}

void MyFrame::UpdateUIBasedOnLanguage()
{
    switch (currentLanguage)
    {
    case Language::ENGLISH:
        analyzeButton->SetLabel(wxT("Analyze Text"));
        openFileButton->SetLabel(wxT("Open File"));
        saveFileButton->SetLabel(wxT("Save File"));
        newFileButton->SetLabel(wxT("New File"));
        revertButton->SetLabel(wxT("Revert Changes"));
        resultLabel->SetLabel(wxT("Analysis Results:"));\
        break;
    case Language::SPANISH:
        analyzeButton->SetLabel(wxT("Analizar Texto"));
        openFileButton->SetLabel(wxT("Abrir Archivo"));
        saveFileButton->SetLabel(wxT("Guardar Archivo"));
        newFileButton->SetLabel(wxT("Nuevo Archivo"));
        revertButton->SetLabel(wxT("Revertir Cambios"));
        resultLabel->SetLabel(wxT("Resultados del Análisis:"));
        break;
    case Language::FRENCH:
        analyzeButton->SetLabel(wxT("Analyser le Texte"));
        openFileButton->SetLabel(wxT("Ouvrir le Fichier"));
        saveFileButton->SetLabel(wxT("Enregistrer le Fichier"));
        newFileButton->SetLabel(wxT("Nouveau Fichier"));
        revertButton->SetLabel(wxT("Annuler les Changements"));
        resultLabel->SetLabel(wxT("Résultats de l'Analyse:"));
        break;
    case Language::POLISH:
        analyzeButton->SetLabel(wxT("Analizuj Tekst"));
        openFileButton->SetLabel(wxT("Otwórz Plik"));
        saveFileButton->SetLabel(wxT("Zapisz Plik"));
        newFileButton->SetLabel(wxT("Nowy Plik"));
        revertButton->SetLabel(wxT("Cofnij Zmiany"));
        resultLabel->SetLabel(wxT("Wyniki Analizy:"));
        break;
    }
}
std::string SprawdzPostawiene(std::string& text, bool capitalizeFirstLetter, bool fixSpacing, bool capitalizeAfterPeriod, bool codingTerms)
{
    std::string wynik;
    initialLikeness = 1.0f;  
    likeness = 1.0f;
    likenessChange = 0.0f;

    std::set<std::string> codingTermsWords = {"float","double", "int", "double"};

    // checks if the first word should be capitalized
    if (capitalizeFirstLetter && !text.empty())
    {
        // extracts the first word
        std::string::size_type firstWordEnd = text.find_first_of(" ,.!;\n");
        std::string firstWord = text.substr(0, firstWordEnd);

        // checks if the first word is a coding term
        bool isCodingTerm = codingTermsWords.find(firstWord) != codingTermsWords.end();
        
        // capitalize the first letter if it's not a coding term
        if (!(codingTerms && isCodingTerm) && std::islower(text[0]))
        {
            text[0] = std::toupper(text[0]);
            wynik += "Converted the first letter to uppercase.\n";
            likeness -= 0.1f;
        }
    }

    // process the entire text
    std::string old_text = text;

    if (fixSpacing)
    {
        text = std::regex_replace(text, std::regex("\\s+([,.])"), "$1");
        text = std::regex_replace(text, std::regex("([,.])([a-zA-Z])"), "$1 $2");
        text = std::regex_replace(text, std::regex("\\s{2,}"), " ");

        if (text != old_text) {
            wynik += "Fixed spaces around punctuation.\n";
            likeness -= 0.1f;
        }
    }

    if (capitalizeAfterPeriod)
    {
        for (size_t i = 0; i < text.length(); ++i)
        {
            if (text[i] == '.' && i < text.length() - 1)
            {
                size_t j = i + 1;
                while (j < text.length() && std::isspace(text[j])) ++j;
                std::string nextWord;
                size_t nextWordStart = j;
                while (j < text.length() && text[j] != ' ' && text[j] != '.' && text[j] != ',' && text[j] != '!' && text[j] != ';') {
                    nextWord += text[j++];
                }
                
                if (!nextWord.empty() && std::islower(nextWord[0]) && 
                    (codingTerms ? codingTermsWords.find(nextWord) == codingTermsWords.end() : true))
                {
                    text[nextWordStart] = std::toupper(text[nextWordStart]);
                    wynik += "Capitalized after period.\n";
                    likeness -= 0.1f;
                }
            }
        }
    }

    // capitalizes the first letter of each sentence unless it's a coding term
    if (capitalizeFirstLetter && !codingTerms)
    {
        std::string new_text;
        std::stringstream ss(text);
        std::string word;
        bool newSentence = true; // flag to see if a new sentence starts
        while (ss >> word)
        {
            if (newSentence && codingTermsWords.find(word) == codingTermsWords.end())
            {
                if (std::islower(word[0]))
                {
                    word[0] = std::toupper(word[0]);
                }
            }
            new_text += word + " ";
            newSentence = (word.back() == '.'); // assumes sentence ends with a period
        }
        text = new_text;
    }

    std::map<std::string, std::string> replacements = {{"teh", "the"}, {"recieve", "receive"}, {"adn", "and"}};
    for (const auto& pair : replacements)
    {
        std::regex word_regex("\\b" + pair.first + "\\b");
        if (std::regex_search(text, word_regex))
        {
            text = std::regex_replace(text, word_regex, pair.second);
            likeness -= 0.1f;
        }
    }

    if (likeness != initialLikeness)
    {
        wynik += "Text analysis completed.\n";
        likenessChange = initialLikeness - likeness;
    }

    return wynik;
}

void MyFrame::OnAnalyze(wxCommandEvent& event)
{
    // stores the current text from textCtrl into previousText
    previousText = textCtrl->GetValue().ToStdString();  // convert wxString to std::string
    
    // retrieves the text from wxTextCtrl and convert it to std::string
    wxString text = textCtrl->GetValue();
    std::string text_std = text.ToStdString(); // convert wxString to std::string

    // starts timing
    auto start = std::chrono::high_resolution_clock::now();
    
    // analyzes the text
    std::string analysis = SprawdzPostawiene(text_std, capitalizeFirstLetter, fixSpacing, capitalizeAfterPeriod, codingTerms);
    
    // stops timing
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    // convertes analysis result from std::string to wxString
    wxString analysisResult = wxString::FromUTF8(analysis.c_str());

    wxString resultText;
    switch (currentLanguage)
    {
        case Language::ENGLISH:
            if (analysisResult.IsEmpty())
                resultText = wxT("Everything is okay");
            else
                resultText = wxT("Text analysis:\n") + analysisResult;
            break;
        
        case Language::POLISH:
            if (analysisResult.IsEmpty())
                resultText = wxT("Wszystko jest okej");
            else
                resultText = wxT("Analiza tekstu:\n") + analysisResult;
            break;
        
        case Language::FRENCH:
            if (analysisResult.IsEmpty())
                resultText = wxT("Tout va bien");
            else
                resultText = wxT("Analyse de texte:\n") + analysisResult;
            break;
        
        case Language::SPANISH:
            if (analysisResult.IsEmpty())
                resultText = wxT("Todo está bien");
            else
                resultText = wxT("Análisis de texto:\n") + analysisResult;
            break;

        default:
            resultText = wxT("Unknown language");
            break;
    }

    resultLabel->SetValue(resultText);
    // showes elapsed time
    wxString timeStr;
    timeStr.Printf("\nTime taken for analysis: %.4f seconds", elapsed.count());
    resultLabel->AppendText(timeStr);

    // updates the wxTextCtrl with the analyzed text
    textCtrl->SetValue(wxString::FromUTF8(text_std.c_str())); // convert std::string back to wxString

    // adjustes the scrollable window
    scrolledWindow->FitInside();
}



// opens a file with UTF-8 encoding
void MyFrame::OnOpenFile(wxCommandEvent& event)
{
    wxFileDialog openFileDialog(this, _("Open Text file"), "", "",
                                "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    previousText = textCtrl->GetValue().ToStdString(); // Stores the current text
    std::ifstream file(openFileDialog.GetPath().ToStdString(), std::ios::in | std::ios::binary);
    if (file.is_open())
    {
        std::string content((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
        textCtrl->SetValue(wxString::FromUTF8(content.c_str()));
    }
}

// saves a file with UTF-8 encoding
void MyFrame::OnSaveFile(wxCommandEvent& event)
{
    // Opens a file dialog to select the file path
    wxFileDialog saveFileDialog(this, _("Save Text file"), "", "",
                                "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    // if the user clicks "Save" 
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;

    // gets the file path from the dialog
    wxString filePath = saveFileDialog.GetPath();
    
    // open the file for writing in binary mode
    std::ofstream file(filePath.ToStdString(), std::ios::out | std::ios::binary);
    
    // checks if the file was successfully opened
    if (file.is_open())
    {
        // gets the text from the wxTextCtrl
        std::string content = textCtrl->GetValue().ToStdString();
        
        // writes the content to the file
        file << content;
        
        // closes the file
        file.close();
    }
    else
    {
        // handles error if the file could not be opened
        wxLogError("Could not open file for saving");
    }
}



void MyFrame::OnNewFile(wxCommandEvent& event)
{
    previousText = textCtrl->GetValue().ToStdString(); // stores the current text
    textCtrl->Clear();
    resultLabel->Clear();
}

void MyFrame::OnRevert(wxCommandEvent& event)
{
    textCtrl->SetValue(previousText);
}

void MyFrame::OnShowSettings(wxCommandEvent& event)
{
    SettingsDialog dialog(this);

    // sets the current values in the settings dialog
    dialog.capitalizeCheckBox->SetValue(capitalizeFirstLetter);
    dialog.spacingCheckBox->SetValue(fixSpacing);
    dialog.periodCheckBox->SetValue(capitalizeAfterPeriod);
    dialog.codingCheckBox->SetValue(codingTerms);

    // sets the selected language in the dialog
    switch (currentLanguage)
    {
    case Language::ENGLISH:
        dialog.langEnglishRadio->SetValue(true);
        break;
    case Language::SPANISH:
        dialog.langSpanishRadio->SetValue(true);
        break;
    case Language::FRENCH:
        dialog.langFrenchRadio->SetValue(true);
        break;
    case Language::POLISH:
        dialog.langPolishRadio->SetValue(true);
        break;
    }

    // shows the dialog and executes the result
    if (dialog.ShowModal() == wxID_OK)
    {
        capitalizeFirstLetter = dialog.capitalizeCheckBox->GetValue();
        fixSpacing = dialog.spacingCheckBox->GetValue();
        capitalizeAfterPeriod = dialog.periodCheckBox->GetValue();
        codingTerms = dialog.codingCheckBox->GetValue();
        currentLanguage = dialog.GetSelectedLanguage();

        UpdateUIBasedOnLanguage();
    }
}


void MyFrame::OnShowDocumentation(wxCommandEvent& event)
{
    DocumentationDialog docDialog(this, currentLanguage);  // passes current language to DocumentationDialog
    docDialog.ShowModal();
}


// application class
class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("TRACE - text review and content evaluation"); // set the name of main window
    frame->Show(true);
    return true;
}